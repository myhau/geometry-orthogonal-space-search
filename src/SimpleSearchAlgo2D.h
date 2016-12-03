#ifndef GEO_PROJ_SIMPLESEARCHALGO2D_H
#define GEO_PROJ_SIMPLESEARCHALGO2D_H


#include "Point.h"
#include <set>
#include <vector>

using namespace std;

/**
 * this one is so easy that one can be sure it works (and be used for tests)
 *
 * use it to compare times of algorithms
 */
template <typename C = double>
class SimpleSearchAlgo2D {
private:

public:


  vector<Point<C>> points;

  SimpleSearchAlgo2D(const vector<Point<C>> & points): points(points) {}

  auto search(double fromX, double toX, double fromY, double toY) {

    set<Point<C>> out;

    for (auto &&item : points) {
      if(fromX <= item.x && item.x <= toX && fromY <= item.y && item.y <= toY) {
        out.insert(item);
      }
    }

    return out;
  }

  auto search(const Rect<C>& rect) {
    return search(rect.xFrom, rect.xTo, rect.yFrom, rect.yTo);
  }
};


#endif //GEO_PROJ_SIMPLESEARCHALGO2D_H
