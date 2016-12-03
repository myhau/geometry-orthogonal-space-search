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

  Point operator-(const Point& rhs) const {
    return Point(x - rhs.x, y - rhs.y);
  }

  Point operator*(double r) const {
    return Point(x * r, y * r);
  }

  Point(double x, double y) : x(x), y(y) {}

  Point(double xy[2]) : x(xy[0]), y(xy[1]) {}

  Point() {}

  friend std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
  }
};

inline Point make_simple_point(double x, double y) {
  return Point(x, y);
}


template<typename T>
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


template<typename T>
inline PointWithData<T> make_point_with_data(const Point &point, T data) {
  return PointWithData<T>(point, data);
}


#endif //GEO_PROJ_POINT_H
