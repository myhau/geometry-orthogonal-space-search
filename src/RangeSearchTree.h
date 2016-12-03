#ifndef GEO_PROJ_RANGESEARCHTREE_H
#define GEO_PROJ_RANGESEARCHTREE_H

#include <vector>
#include <set>
#include <functional>
#include <boost/format.hpp>
#include <iostream>
#include "Node.h"
#include <memory>
#include "../src/util/PrettyPrintNode.h"

using std::vector;
using std::set;
using std::function;
using std::iterator;
using std::copy;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;

template<typename T, typename C = double>
using KeyFunction = function<C(T)>;


template<typename T, typename C = double>
class RangeSearchTree {
private:

  using TreeKeyFunction = KeyFunction<T, C>;
  using NodePtr = Node<T, C> *;
  using Vec = vector<T>;
  using Set = set<T>;

  bool elementsAreSorted;

  TreeKeyFunction keyF;

  NodePtr tree;

  NodePtr buildSubTree(const Vec &els, size_t beg, size_t end) {

    if (beg > end) {
      return nullptr;
    }

    if (beg == end) {
      auto value = els[beg];
      return make_node_ptr(keyF(value), value);
    }

    // TODO: probably we can simplify this code here
    auto medPos = beg + end;
    auto medianPosition = medPos / 2;
    C median;

    auto keyMedian1 = keyF(els[medianPosition]);
    auto keyMedian2 = keyF(els[medianPosition + 1]);

    if (medPos % 2 == 0) {
      median = keyMedian1;
    } else {
      median = (keyMedian1 + keyMedian2) / 2;
    }

    auto firstEndI = medianPosition - 1;
    if (keyMedian1 <= median) {
      firstEndI = medianPosition;
    }
    if (keyMedian2 <= median) {
      firstEndI = medianPosition + 1;
    }

    auto medianNode = make_node_ptr<T>(median);

    if (firstEndI >= beg) {
      if (firstEndI == end || firstEndI + 1 == beg) {
        throw logic_error("Should not happend, will be solved");
      }
      medianNode->left = buildSubTree(els, beg, firstEndI);
      medianNode->right = buildSubTree(els, firstEndI + 1, end);
    }

    return medianNode;
  }

  NodePtr buildTree(const Vec &elements) {
    if (elements.empty()) return nullptr;

    if (!elementsAreSorted) {
      Vec sortedVec(elements);
      std::sort(sortedVec.begin(), sortedVec.end(),
                [this](const T &p1, const T &p2) { return keyF(p2) > keyF(p1); });
      return buildSubTree(elements, 0, elements.size() - 1);
    } else {
      return buildSubTree(elements, 0, elements.size() - 1);
    }
  }

  void collectWholeSubTree(NodePtr root, Set &collectTo) const {
    if (root == nullptr) return;
    if (root->isLeaf) {
      collectTo.insert(root->value);
    } else {
      collectWholeSubTree(root->left, collectTo);
      collectWholeSubTree(root->right, collectTo);
    }
  }

  void collectWholeSubTree(NodePtr root, NodePtr additionalNode, Set &collectTo) const {
    collectWholeSubTree(root, collectTo);
    if (additionalNode->isLeaf) {
      collectTo.insert(additionalNode->value);
    }
  }

  enum VSplitSubtree {
    LEFT, RIGHT
  };

  void
  collectAll(NodePtr root, C from, C to, VSplitSubtree subtreeType, Set &collector) const {
    if (root == nullptr) return;

    NodePtr iterNode;

    if (subtreeType == LEFT) {
      iterNode = root->left;
    } else {
      iterNode = root->right;
    }

    while (!iterNode->isLeaf) {
      if (subtreeType == LEFT) {

        if (iterNode->key >= from) {
          collectWholeSubTree(iterNode->right, iterNode, collector);
          iterNode = iterNode->left;
        } else {
          iterNode = iterNode->right;
        }

      } else {

        if (iterNode->key <= to) {
          collectWholeSubTree(iterNode->left, iterNode, collector);
          iterNode = iterNode->right;
        } else {
          iterNode = iterNode->left;
        }

      }
    }
    if (from <= iterNode->key && iterNode->key <= to) collectWholeSubTree(iterNode, collector);
  }

  NodePtr findVSplit(NodePtr tree, C from, C to) const {
    NodePtr subTree = tree;
    while (subTree != nullptr && !subTree->isLeaf && (subTree->key >= to || subTree->key < from)) {
      if (subTree->key >= to) {
        subTree = subTree->left;
      } else {
        subTree = subTree->right;
      }
    }

    return subTree;
  }

public:

  Set search(C keyFrom, C keyTo) const {
    Set collector;
    search(keyFrom, keyTo, collector);
    return collector;
  }

  void search(C keyFrom, C keyTo, set<T> &collector) const {
    if (keyFrom > keyTo) {
      auto err = (boost::format("Invalid arguments from %s to %s") % keyFrom % keyTo).str();
      throw std::invalid_argument(err);
    }

    if (tree == nullptr) return;

    NodePtr vSplitTree = findVSplit(tree, keyFrom, keyTo);

    if (vSplitTree == nullptr) return;

    if (vSplitTree->isLeaf) {
      if (keyFrom <= vSplitTree->key && vSplitTree->key <= keyTo) {
        collector.insert(vSplitTree->value);
      }
    } else {
      collectAll(vSplitTree, keyFrom, keyTo, LEFT, collector);
      collectAll(vSplitTree, keyFrom, keyTo, RIGHT, collector);
    }
  }

  RangeSearchTree(const vector<T> &els, TreeKeyFunction keyF, bool elementsAreSorted = false) :
          keyF(keyF),
          elementsAreSorted(elementsAreSorted),
          tree(buildTree(els)) {
  }


  RangeSearchTree() : keyF([](T a) { return 0.0; }), tree(nullptr) {}


  friend std::ostream &operator<<(std::ostream &os, const RangeSearchTree &tree) {
    prettyPrint(os, tree.tree);
    return os;
  }

  virtual ~RangeSearchTree() {
    destruct(tree);
  }
};


template<typename T, typename C = double>
RangeSearchTree<T, C> make_range_search_tree(const vector<T> &els, KeyFunction<T, C> keyF) {
  return RangeSearchTree<T, C>(els, keyF);
}

template<typename T, typename C = double>
shared_ptr<RangeSearchTree<T, C>>
make_range_search_tree_shared_ptr(const vector<T> &els, KeyFunction<T, C> keyF, bool sorted = false) {
  return shared_ptr<RangeSearchTree<T, C>>(new RangeSearchTree<T, C>(els, keyF));
}


#endif //GEO_PROJ_RANGESEARCHTREE_H
