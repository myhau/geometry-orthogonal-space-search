#ifndef GEO_PROJ_RECT_H
#define GEO_PROJ_RECT_H


struct Rect {
  double yFrom;
  double yTo;
  double xFrom;
  double xTo;

  Rect(double xFrom, double xTo, double yFrom, double yTo) : yFrom(yFrom), yTo(yTo), xFrom(xFrom), xTo(xTo) {}
};


#endif //GEO_PROJ_RECT_H
