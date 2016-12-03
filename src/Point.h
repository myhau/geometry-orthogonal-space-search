#ifndef GEO_PROJ_POINT_H
#define GEO_PROJ_POINT_H


#include <ostream>
#include "ComparableTuple.h"

template<typename C = double>
struct Point {
  C x;
  C y;

  bool operator==(const Point &rhs) const {
    return x == rhs.x &&
           y == rhs.y;
  }

  bool operator!=(const Point &rhs) const {
    return !(rhs == *this);
  }

  bool operator<(const Point &rhs) const {
    if (x < rhs.x)
      return true;
    if (rhs.x < x)
      return false;
    return y < rhs.y;
  }

  bool operator>(const Point &rhs) const {
    return rhs < *this;
  }

  bool operator<=(const Point &rhs) const {
    return !(rhs < *this);
  }

  bool operator>=(const Point &rhs) const {
    return !(*this < rhs);
  }

  Point operator+(const Point &rhs) const {
    return Point(x + rhs.x, y + rhs.y);
  }

  Point operator-() const {
    return Point(-x, -y);
  }

  Point operator-(const Point &rhs) const {
    return Point(x - rhs.x, y - rhs.y);
  }

  Point operator*(double r) const {
    return Point(x * r, y * r);
  }

  Point(C x, C y) : x(x), y(y) {}

  Point(C xy[2]) : x(xy[0]), y(xy[1]) {}

  Point() {}

  friend std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
  }

  static C getX(const Point &p) {
    return p.x;
  }

  static C getY(const Point &p) {
    return p.y;
  }
};

template <typename C = double>
inline Point<C> point(C x, C y) {
  return Point<C>(x, y);
}



inline static Point<ComparableTuple> toUniquePoint(Point<double> p) {
  return point(ComparableTuple(p.x, p.y), ComparableTuple(p.y, p.x));
}

template<typename T, typename C = double>
struct PointWithData {
  Point<C> point;
  T data;

  PointWithData(const Point<C> &point, T data) : point(point), data(data) {}

  PointWithData() {}

  bool operator==(const PointWithData &rhs) const {
    return point == rhs.point &&
           data == rhs.data;
  }

  bool operator!=(const PointWithData &rhs) const {
    return !(rhs == *this);
  }

  bool operator<(const PointWithData &rhs) const {
    if (point < rhs.point)
      return true;
    if (rhs.point < point)
      return false;
    return data < rhs.data;
  }

  bool operator>(const PointWithData &rhs) const {
    return rhs < *this;
  }

  bool operator<=(const PointWithData &rhs) const {
    return !(rhs < *this);
  }

  bool operator>=(const PointWithData &rhs) const {
    return !(*this < rhs);
  }

  friend std::ostream &operator<<(std::ostream &os, const PointWithData &data) {
    os << "point: (" << data.point << ") data: (" << data.data << ")";
    return os;
  }

};


template<typename T, typename C = double>
inline PointWithData<T, C> make_point_with_data(const Point<C> &point, T data) {
  return PointWithData<T, C>(point, data);
}

template<typename T, typename C = double>
C getX(const PointWithData<T, C> &p) {
  return p.point.x;
}

template<typename T, typename C = double>
C getY(const PointWithData<T, C> &p) {
  return p.point.y;
}


#endif //GEO_PROJ_POINT_H
