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
class SimpleSearchAlgo2D {
public:

  vector<Point> points;

  SimpleSearchAlgo2D(const vector<Point> & points): points(points) {}

  set<Point> search(double fromX, double toX, double fromY, double toY) {

    set<Point> out;

    for (auto &&item : points) {
      if(fromX <= item.x && item.x <= toX && fromY <= item.y && item.y <= toY) {
        out.insert(item);
      }
    }

    return out;
  }

  set<Point> search(const Rect& rect) {
    return search(rect.xFrom, rect.xTo, rect.yFrom, rect.yTo);
  }
};


#endif //GEO_PROJ_SIMPLESEARCHALGO2D_H
