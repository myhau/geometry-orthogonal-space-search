#ifndef GEO_PROJ_RANGESEARCHTREE2D_H
#define GEO_PROJ_RANGESEARCHTREE2D_H

#include<functional>
#include<set>
#include<vector>
#include <algorithm>
#include <boost/format.hpp>
#include <memory>
#include "Node.h"
#include "Point.h"
#include "SortedPoints.h"
#include "RangeSearchTree.h"
#include "Rect.h"


using std::function;
using std::vector;
using std::set;
using std::transform;
using std::make_shared;
using std::shared_ptr;


template<typename T>
class RangeSearchTree2D {
private:

  using KeyFunction = function<Point(const T &)>;
  using NodeValue = shared_ptr<RangeSearchTree<PointWithData<T>>>;
  using NodePtr = Node<NodeValue> *;
  using Vec = vector<T>;
  using Set = set<T>;

  KeyFunction keyF;
public:
  NodePtr tree;
private:

  NodePtr buildTree(const vector<T> &elements) {
    if (elements.empty()) return nullptr;

    vector<PointWithData<T>> pointsWithData(elements.size());
    auto f = [this](const T &data) { return make_point_with_data(keyF(data), data); };
    transform(elements.begin(), elements.end(), pointsWithData.begin(), f);

    auto sortedPoints = make_sorted_points(pointsWithData);

    return buildSubTree(pointsWithData, 0, sortedPoints.size() - 1);
  }

  NodePtr buildSubTree(const SortedPoints<T> &preSorted, size_t xBeg, size_t xEnd) {

    if (xBeg > xEnd) {
      return nullptr;
    }

    if (xBeg == xEnd) {
      auto value = preSorted.getX(xBeg);
      vector<PointWithData<T>> values = {value};
      NodePtr node =
              make_node_ptr(
                      getX(value),
                      make_range_search_tree_shared_ptr<PointWithData<T>>(values, getY<T>, true),
                      true
              );

      return node;
    }

    auto associatedStructureData = preSorted.getAllY(xBeg, xEnd);
    auto associatedStructure =
            make_range_search_tree_shared_ptr<PointWithData<T>>(
                    associatedStructureData,
                    getY<T>,
                    true
            );

    // TODO: probably we can simplify this code here
    auto almostMedianPosition = xBeg + xEnd;
    auto medianPosition = almostMedianPosition / 2;

    auto nextIterationMiddleCandidate = medianPosition - 1;
    double median;
    if (almostMedianPosition % 2 == 0) {
      median = getX(preSorted.getX(medianPosition));
      nextIterationMiddleCandidate = medianPosition;
    } else {
      auto medVal1 = getX(preSorted.getX(medianPosition));
      auto medVal2 = getX(preSorted.getX(medianPosition + 1));
      median = (medVal1 + medVal2) / 2;
      if (medVal1 <= median) {
        nextIterationMiddleCandidate = medianPosition;
      }
      if (medVal2 <= median) {
        nextIterationMiddleCandidate = medianPosition + 1;
      }
    }

    NodePtr medianNode = make_node_ptr(median, associatedStructure, false);

    if (nextIterationMiddleCandidate >= xBeg) {
      if (nextIterationMiddleCandidate == xEnd || nextIterationMiddleCandidate + 1 == xBeg) {
        throw logic_error("Should not happend, will be solved");
      }
      medianNode->left = buildSubTree(preSorted, xBeg, nextIterationMiddleCandidate);
      medianNode->right = buildSubTree(preSorted, nextIterationMiddleCandidate + 1, xEnd);
    }

    return medianNode;
  }

  NodePtr findVSplit(NodePtr tree, const Rect &area) const {
    NodePtr subTree = tree;
    while (!subTree->isLeaf && (subTree->key >= area.xTo || subTree->key < area.xFrom)) {
      if (subTree->key >= area.xTo) {
        subTree = subTree->left;
      } else {
        subTree = subTree->right;
      }
    }
    return subTree;
  }

  enum VSplitSubtree {
    LEFT, RIGHT
  };

  void collectAll(NodePtr tree, const Rect &area, VSplitSubtree subtreeType,
                  set<PointWithData<T>> &collector) const {

    NodePtr iterNode;

    if (subtreeType == LEFT) {
      iterNode = tree->left;
    } else {
      iterNode = tree->right;
    }

    while (!iterNode->isLeaf) {

      if (subtreeType == LEFT) {

        if (iterNode->key >= area.xFrom) {
          collectFromAssociatedStructure(iterNode->right, area, collector);
          iterNode = iterNode->left;
        } else {
          iterNode = iterNode->right;
        }

      } else {

        if (iterNode->key <= area.xTo) {
          collectFromAssociatedStructure(iterNode->left, area, collector);
          iterNode = iterNode->right;
        } else {
          iterNode = iterNode->left;
        }

      }
    }
    if (area.xFrom <= iterNode->key && iterNode->key <= area.xTo) {
      collectFromAssociatedStructure(iterNode, area, collector);
    }
  }

  void
  collectFromAssociatedStructure(NodePtr tree, const Rect &area, set<PointWithData<T>> &collector) const {
    if (tree == nullptr) return;
    auto yDimensionSearchTree = tree->value.get();
    yDimensionSearchTree->search(area.yFrom, area.yTo, collector);
  }

public:


  RangeSearchTree2D(const vector<T> &els, KeyFunction keyF) :
          keyF(keyF),
          tree(buildTree(els)) {
  }

  set<T> search(double fromX, double toX, double fromY, double toY) const {
    return search(Rect(fromX, toX, fromY, toY));
  }

  set<T> search(const Rect &area) const {

    if (area.xFrom > area.xTo) {
      auto err = (boost::format("Invalid arguments fromX, toX from %s to %s") % area.xFrom % area.yTo).str();
      throw std::invalid_argument(err);
    } else if (area.yFrom > area.yTo) {
      auto err = (boost::format("Invalid arguments fromY, toY from %s to %s") % area.yFrom % area.yTo).str();
      throw std::invalid_argument(err);
    }

    set<T> out;

    if (tree == nullptr) return out;

    NodePtr vSplitTree = findVSplit(tree, area);
    if (vSplitTree == nullptr) return out;

    set<PointWithData<T>> collector;

    if (vSplitTree->isLeaf) {
      if(area.xFrom <= vSplitTree->key && vSplitTree->key <= area.xTo) {
        collectFromAssociatedStructure(vSplitTree, area, collector);
      }
    } else {
      collectAll(vSplitTree, area, LEFT, collector);
      collectAll(vSplitTree, area, RIGHT, collector);
    }

    for (auto &&item : collector) {
      out.insert(item.data);
    }

    return out;
  }


  virtual ~RangeSearchTree2D() {
    destruct(tree);
  }

};

static RangeSearchTree2D<Point> searchTreeOfPoints(const vector<Point> &els) {
  return RangeSearchTree2D<Point>(els, [](const Point &a) { return a; });
}


#endif //GEO_PROJ_RANGESEARCHTREE2D_H
