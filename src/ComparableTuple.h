#ifndef GEO_PROJ_COMPARABLETUPLE_H
#define GEO_PROJ_COMPARABLETUPLE_H


#include <ostream>

class ComparableTuple {
  double c1;
  double c2;

public:
  ComparableTuple(double c1, double c2) : c1(c1), c2(c2) {}

  ComparableTuple() {}

public:
  bool operator==(const ComparableTuple &rhs) const {
    return c1 == rhs.c1 &&
           c2 == rhs.c2;
  }

  bool operator!=(const ComparableTuple &rhs) const {
    return !(rhs == *this);
  }

  friend std::ostream &operator<<(std::ostream &os, const ComparableTuple &tuple) {
    os << "(" << tuple.c1 << "|" << tuple.c2 << ")";
    return os;
  }

  bool operator<(const ComparableTuple &rhs) const {
    if (c1 < rhs.c1)
      return true;
    if (rhs.c1 < c1)
      return false;
    return c2 < rhs.c2;
  }

  bool operator>(const ComparableTuple &rhs) const {
    return rhs < *this;
  }

  bool operator<=(const ComparableTuple &rhs) const {
    return !(rhs < *this);
  }

  bool operator>=(const ComparableTuple &rhs) const {
    return !(*this < rhs);
  }

  ComparableTuple operator+(const ComparableTuple &rhs) const {
    return ComparableTuple(c1 + rhs.c1, c2 + rhs.c2);
  }

  ComparableTuple operator-(const ComparableTuple &rhs) const {
    return ComparableTuple(c1 - rhs.c1, c2 - rhs.c2);
  }

  ComparableTuple operator-() const {
    return ComparableTuple(-c1, -c2);
  }

  ComparableTuple operator*(const double& d) const {
    return ComparableTuple(c1 * d, c2 * d);
  }

  ComparableTuple operator/(const double& d) const {
    return ComparableTuple(c1 / d, c2 / d);
  }

  double to_double() {
    return c1;
  }


};


#endif //GEO_PROJ_COMPARABLETUPLE_H
