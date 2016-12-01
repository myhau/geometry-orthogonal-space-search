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

template<typename T>
using KeyFunction = function<double(T)>;


template<typename T>
class RangeSearchTree {
private:

  using TreeKeyFunction = KeyFunction<T>;
  using NodePtr = Node<T> *;
  using Vec = vector<T>;
  using Set = set<T>;

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
    double median;

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
      if(firstEndI != end) {
        medianNode->left = buildSubTree(els, beg, firstEndI);
      }
      if(firstEndI + 1 != beg) {
        medianNode->right = buildSubTree(els, firstEndI + 1, end);
      }
    }

    return medianNode;
  }

  NodePtr buildTree(const Vec &elements) {
    Vec sortedVec(elements);
    std::sort(sortedVec.begin(), sortedVec.end(), [this](const T &p1, const T &p2) {return keyF(p2) > keyF(p1);});

    return buildSubTree(sortedVec, 0, sortedVec.size() - 1);
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

  void collectAll(NodePtr root, double from, double to, VSplitSubtree subtreeType, Set &collector) const {
    if (root == nullptr) return;

    NodePtr iterNode;

    if (subtreeType == LEFT) {
      iterNode = root->left;
    } else {
      iterNode = root->right;
    }

    if (iterNode == nullptr) return;
    if (iterNode->isLeaf) {
      if (iterNode->key >= from && iterNode->key <= to) collectWholeSubTree(iterNode, collector);
    }

    while (iterNode != nullptr) {
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

  }

  NodePtr findVSplit(NodePtr tree, double from, double to) const {
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

  RangeSearchTree(const vector<T> &els, TreeKeyFunction keyF) :
          keyF(keyF),
          tree(buildTree(els)) {
  }

  RangeSearchTree() : keyF([](T a) { return 0.0; }), tree(nullptr) {}


//  RangeSearchTree(const vector<T>& els, function<double(T*)> keyF) {
//    vec vec2(els.size());
//    std::transform(els.begin(), els.end(), vec2.begin(), [](T& a) -> T* { return &a; });
//    RangeSearchTree(vec2, keyF);
//  }


  Set search(double keyFrom, double keyTo) const {
    Set collector;
    search(keyFrom, keyTo, collector);
    return collector;
  }

  void search(double keyFrom, double keyTo, set<T>& collector) const {
    if (keyFrom > keyTo) {
      auto err = (boost::format("Invalid arguments from %s to %s") % keyFrom % keyTo).str();
      throw std::invalid_argument(err);
    }

    if(tree == nullptr) return;

    NodePtr vSplitTree = findVSplit(tree, keyFrom, keyTo);

    if (vSplitTree == nullptr) return;

    if (vSplitTree->isLeaf && (vSplitTree->key >= keyFrom && vSplitTree->key <= keyTo)) {
      collector.insert(vSplitTree->value);
    } else {
      collectAll(vSplitTree, keyFrom, keyTo, LEFT, collector);
      collectAll(vSplitTree, keyFrom, keyTo, RIGHT, collector);
    }
  }


  friend std::ostream &operator<<(std::ostream &os, const RangeSearchTree &tree) {
    prettyPrint(os, tree.tree);
    return os;
  }

  virtual ~RangeSearchTree() {
    destruct(tree);
  }
};


template<typename T>
RangeSearchTree<T> make_range_search_tree(const vector<T> &els, KeyFunction<T> keyF) {
  return RangeSearchTree<T>(els, keyF);
}

template<typename T>
shared_ptr<RangeSearchTree<T>> make_range_search_tree_shared_ptr(const vector<T> &els, KeyFunction<T> keyF) {
  return shared_ptr<RangeSearchTree<T>>(new RangeSearchTree<T>(els, keyF));
}



#endif //GEO_PROJ_RANGESEARCHTREE_H
