#ifndef GEO_PROJ_RECT_H
#define GEO_PROJ_RECT_H


#include <ostream>

template<typename C = double>
struct Rect {
  C yFrom;
  C yTo;
  C xFrom;
  C xTo;

  Rect(C xFrom, C xTo, C yFrom, C yTo) : yFrom(yFrom), yTo(yTo), xFrom(xFrom), xTo(xTo) {}

  Rect(const Point<C> &p1, const Point<C> &p2) {
    xFrom = min(p1.x, p2.x);
    xTo = max(p1.x, p2.x);
    yFrom = min(p1.y, p2.y);
    yTo = max(p1.y, p2.y);
  }

  friend ostream &operator<<(ostream &os, const Rect &rect) {
    os << " xFrom: " << rect.xFrom << " xTo: " << rect.xTo << "yFrom: " << rect.yFrom << " yTo: " << rect.yTo;
    return os;
  }

  bool contains(const Point<C> &p) {
    return xFrom >= p.x && xTo <= p.x && yFrom >= p.y && yTo <= p.y;
  }
};

template<typename C = double>
Rect<C> make_rect(C xFrom, C xTo, C yFrom, C yTo) {
  return Rect<C>(xFrom, xTo, yFrom, yTo);
}

template<typename C = double>
Rect<C> make_rect(Point<C> p1, Point<C> p2) {
  return Rect<C>(p1, p2);
}


#endif //GEO_PROJ_RECT_H
