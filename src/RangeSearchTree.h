#ifndef GEO_PROJ_RANGESEARCHTREE_H
#define GEO_PROJ_RANGESEARCHTREE_H

#include <vector>
#include <set>
#include <functional>
#include <boost/format.hpp>
#include <iostream>
#include "Node.h"
#include "../src/util/PrettyPrintNode.h"

using std::vector;
using std::set;
using std::function;
using std::iterator;
using std::copy;
using std::cout;
using std::endl;

template<typename T>
class RangeSearchTree {
private:

  using KeyFunction = function<double(T)>;
  using NodePtr = Node<T> *;
  using Vec = vector<T>;
  using Set = set<T>;

  KeyFunction keyF;

  NodePtr tree;

  NodePtr buildSubTree(const Vec &els, long beg, long end) {

    if (beg > end) {
      return nullptr;
    }

    if (beg == end) {
      auto value = els[beg];
      return new Node<T>(keyF(value), value);
    }

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


    auto medianNode = new Node<T>(median);

    if (firstEndI >= beg) {
      medianNode->left = buildSubTree(els, beg, firstEndI);
      medianNode->right = buildSubTree(els, firstEndI + 1, end);
    }

    return medianNode;
  }

  NodePtr buildTree(const Vec &elements) {
    Vec sortedVec(elements);
    std::sort(sortedVec.begin(), sortedVec.end(), );

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

  void collectAll(NodePtr root, double from, double to, VSplitSubtree subtree, Set &collectTo) const {
    if (root == nullptr) return;

    NodePtr iterNode;

    if (subtree == LEFT) {
      iterNode = root->left;
    } else {
      iterNode = root->right;
    }

    if (iterNode == nullptr) return;
    if (iterNode->isLeaf) {
      if (iterNode->key >= from && iterNode->key <= to) collectWholeSubTree(iterNode, collectTo);
    }

    while (iterNode != nullptr) {
      if (subtree == LEFT) {

        if (iterNode->key >= from) {
          collectWholeSubTree(iterNode->right, iterNode, collectTo);
          iterNode = iterNode->left;
        } else {
          iterNode = iterNode->right;
        }

      } else {

        if (iterNode->key <= to) {
          collectWholeSubTree(iterNode->left, iterNode, collectTo);
          iterNode = iterNode->right;
        } else {
          iterNode = iterNode->left;
        }

      }
    }

  }

  NodePtr findVSplit(NodePtr tree, double from, double to) const {
    NodePtr subTree = tree;
    while (!subTree->isLeaf && (subTree->key >= to || subTree->key < from)) {
      if (subTree->key >= to) {
        subTree = subTree->left;
      } else {
        subTree = subTree->right;
      }
    }

    return subTree;
  }

public:

  RangeSearchTree(const vector<T> &els, KeyFunction keyF) :
          keyF(keyF),
          tree(buildTree(els)) {
  }

  RangeSearchTree() : keyF([](T a){return 0.0;}), tree(nullptr) {}


//  RangeSearchTree(const vector<T>& els, function<double(T*)> keyF) {
//    vec vec2(els.size());
//    std::transform(els.begin(), els.end(), vec2.begin(), [](T& a) -> T* { return &a; });
//    RangeSearchTree(vec2, keyF);
//  }


  Set search(double keyFrom, double keyTo) const {
    if (keyFrom > keyTo) {
      auto err = (boost::format("Invalid arguments from %s to %s") % keyFrom % keyTo).str();
      throw std::invalid_argument(err);
    }

    NodePtr vSplitTree = findVSplit(tree, keyFrom, keyTo);

    Set collectTo;

    if(vSplitTree == nullptr) return collectTo;

    if(vSplitTree->isLeaf && (vSplitTree->key >= keyFrom && vSplitTree->key <= keyTo)) {
      collectTo.insert(vSplitTree->value);
      return collectTo;
    } else {
      collectAll(vSplitTree, keyFrom, keyTo, LEFT, collectTo);
      collectAll(vSplitTree, keyFrom, keyTo, RIGHT, collectTo);
    }

    return collectTo;
  }

  virtual ~RangeSearchTree() {
    destruct(tree);
  }
};


#endif //GEO_PROJ_RANGESEARCHTREE_H
