#ifndef GEO_PROJ_RECT_H
#define GEO_PROJ_RECT_H


struct Rect {
  double yFrom;
  double yTo;
  double xFrom;
  double xTo;

  Rect(double xFrom, double xTo, double yFrom, double yTo) : yFrom(yFrom), yTo(yTo), xFrom(xFrom), xTo(xTo) {}
  
  Rect(const Point& p1, const Point& p2) {
    xFrom = min(p1.x, p2.x);
    xTo = max(p1.x, p2.x);
    yFrom = min(p1.y, p2.y);
    yTo = max(p1.y, p2.y);
  }
};


#endif //GEO_PROJ_RECT_H
