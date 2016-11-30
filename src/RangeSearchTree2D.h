#ifndef GEO_PROJ_RANGESEARCHTREE2D_H
#define GEO_PROJ_RANGESEARCHTREE2D_H

#include<functional>
#include<set>
#include<vector>
#include <algorithm>
#include <boost/format.hpp>
#include "Node.h"
#include "Point.h"
#include "SortedPoints.h"
#include "RangeSearchTree.h"

using std::function;
using std::vector;
using std::set;
using std::transform;

template<typename T>
class RangeSearchTree2D {
private:

  using KeyFunction = function<Point(const T&)>;
  using NodePtr = Node<RangeSearchTree<PointWithData<T>>> *;
  using Vec = vector<T>;
  using Set = set<T>;

  KeyFunction keyF;
public:
  NodePtr tree;
private:

  NodePtr findVSplit(NodePtr tree, double from, double to) {
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

  NodePtr buildTree(const vector<T> &elements) {
    vector<PointWithData<T>> pointsWithData(elements.size());
    auto f = [this](const T& data){ return PointWithData<T>(keyF(data), data);};
    transform(elements.begin(), elements.end(), pointsWithData.begin(), f);

    auto sortedPoints = SortedPoints<T>(pointsWithData);

    return buildSubTree(pointsWithData, 0, sortedPoints.size() - 1);
  }

  NodePtr buildSubTree(const SortedPoints<T> &preSorted, long xBeg, long xEnd) {


    if(xBeg > xEnd) {
      return nullptr;
    }

    auto toYCoordMapper = [](const PointWithData<T>& pointAndData){return pointAndData.point.y;};

    auto toXCoordMapper = [](const PointWithData<T>& pointAndData){return pointAndData.point.x;};

    if(xBeg == xEnd) {
      auto value = preSorted.getX(xBeg);
      vector<PointWithData<T>> values = {value};
      return new Node<RangeSearchTree<PointWithData<T>>>(toXCoordMapper(value), RangeSearchTree<PointWithData<T>>(values, toYCoordMapper), true);
    }

    auto associatedStructureData = preSorted.getAllY(xBeg, xEnd);

    auto associatedStructure = RangeSearchTree<PointWithData<T>>(associatedStructureData, toYCoordMapper);

    auto medPos = xBeg + xEnd;
    auto medianPosition = medPos / 2;
    double median;


    auto firstEndI = medianPosition - 1;

    if (medPos % 2 == 0) {
      median = toXCoordMapper(preSorted.getX(medPos));
      firstEndI = medianPosition;
    } else {
      auto medVal1 = toXCoordMapper(preSorted.getX(medPos));
      auto medVal2 = toXCoordMapper(preSorted.getX(medPos + 1));
      median = medVal1 + medVal2;
      if(medVal1 <= median) {
        firstEndI = medianPosition;
      }
      if(medVal2 <= median) {
        firstEndI = medianPosition + 1;
      }
    }

    auto medianNode = new Node<RangeSearchTree<PointWithData<T>>>(median, associatedStructure, false);

    if (firstEndI >= xBeg) {
      medianNode->left = buildSubTree(preSorted, xBeg, firstEndI);
      medianNode->right = buildSubTree(preSorted, firstEndI + 1, xEnd);
    }

    return medianNode;
  }

public:



  RangeSearchTree2D(const vector<T> &els, KeyFunction keyF) :
          keyF(keyF),
          tree(buildTree(els)) {
  }

  vector<T> search(double x1, double x2, double y1, double y2) const {

    if (x1 > x2) {
      auto err = (boost::format("Invalid arguments x1, x2 from %s to %s") % x1 % x2).str();
      throw std::invalid_argument(err);
    } else if(y1 > y2) {
      auto err = (boost::format("Invalid arguments y1, y2 from %s to %s") % y1 % y2).str();
      throw std::invalid_argument(err);
    }

//    NodePtr vSplitTree = findVSplit(tree, );

    vector<T> collectTo;

//    if(vSplitTree == nullptr) return collectTo;
//
//    if(vSplitTree->isLeaf && (vSplitTree->key >= keyFrom && vSplitTree->key <= keyTo)) {
//      collectTo.insert(vSplitTree->value);
//      return collectTo;
//    } else {
//      collectAll(vSplitTree, keyFrom, keyTo, LEFT, collectTo);
//      collectAll(vSplitTree, keyFrom, keyTo, RIGHT, collectTo);
//    }

    return collectTo;
  }

  virtual ~RangeSearchTree2D() {
    destruct(tree);
  }
};



#endif //GEO_PROJ_RANGESEARCHTREE2D_H
