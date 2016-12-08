#ifndef POINT_HPP
#define POINT_HPP

#include "common.hpp"

#include <cmath>

namespace gogui {

class Point : public GeoObject
{
public:
    double x, y;

    constexpr Point(const double & x1, const double & y1)
    : x(x1), y(y1)
    {}

    bool operator ==(const Point & that) const {
        return compareDouble(this->x, that.x) && compareDouble(this->y, that.y);
    }

    bool operator !=(const Point & that) const {
        return !(*this == that);
    }

    bool operator <(const Point & that) const {
        if(x != that.x)
            return x < that.x;
        return y < that.y;
    }

    double distance (const Point & p) const {
        return sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y));
    }

    template<class T>
    double distance (const T& that) const {
        return that.distance(*this);
    }
};

}
#endif //POINT_HPP
