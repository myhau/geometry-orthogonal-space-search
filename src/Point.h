#ifndef GEO_PROJ_POINT_H
#define GEO_PROJ_POINT_H


#include <ostream>

struct Point {
  double x;
  double y;

  bool operator==(const Point &rhs) const {
    return x == rhs.x &&
           y == rhs.y;
  }

  bool operator!=(const Point &rhs) const {
    return !(rhs == *this);
  }

  Point(double x, double y) : x(x), y(y) {}

  Point() {}

  friend std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
  }
};


template <typename T>
struct PointWithData {
  Point point;
  T data;

  PointWithData(const Point &point, T data) : point(point), data(data) {}

  PointWithData() {}

  bool operator==(const PointWithData &rhs) const {
    return point == rhs.point &&
           data == rhs.data;
  }

  bool operator!=(const PointWithData &rhs) const {
    return !(rhs == *this);
  }

  friend std::ostream &operator<<(std::ostream &os, const PointWithData &data) {
    os << "point: (" << data.point << ") data: (" << data.data << ")";
    return os;
  }

};


#endif //GEO_PROJ_POINT_H
