#ifndef LINE_H
#define LINE_H
#define PI 3.141592653
#define EPSILON 1.0e-12
#include "point.hpp"
#include <cmath>

namespace gogui {

class Line : public GeoObject
{
private:
    struct Parameters {
        double A;           //Ax+By+C=0
        double B;
        double C;
    };

    Parameters parameters;

    double getA () {
        return point1.y - point2.y;
    }

    double getB () {
        return point2.x - point1.x;
    }

    double getC () {
        return (point1.x * point2.y) - (point2.x * point1.y);
    }

    bool compareDouble (const double a, const double b) const {
        return fabs(a - b) < EPSILON;
    }

public:
    Line(const Point& p1, const Point& p2)
    : point1(p1), point2(p2) {
        parameters.A=getA();
        parameters.B=getB();
        parameters.C=getC();
    }
    const Point point1;
    const Point point2;
private:
    bool isEqualToWithOrder(const Line & that) const {
        return (point1 == that.point1) && (point2 == that.point2);
    }

    bool isEqualToReversed(const Line & that) const {
        return (point1 == that.point2) && (point2 == that.point1);
    }

public:
    bool operator ==(const Line & that) const {
        return isEqualToWithOrder(that) || isEqualToReversed(that);
    }

    bool operator !=(const Line & that) const {
        return !(*this == that);
    }


    bool isParallel (const Line & line) const {
        return compareDouble(parameters.A, line.parameters.A) &&
                            compareDouble(parameters.B, line.parameters.B);
    }

    bool isPerpendicular (const Line & line) const {
        return compareDouble(parameters.A*line.parameters.A, -parameters.B*line.parameters.B);
    }

    double distance (const Line & line) const {
        if(!this->isParallel(line))
            return 0;
        return (fabs(parameters.C - line.parameters.C))/
                (sqrt(parameters.A*parameters.A + parameters.B*parameters.B));
    }

    double distance (const Point & p) const {
        return fabs(parameters.A*p.x + parameters.B*p.y + parameters.C)/
                        sqrt(parameters.A*parameters.A + parameters.B*parameters.B);
    }

    double angleBetweenLines (const Line & line) const {
        if(isPerpendicular(line)) return PI/2;
        return atan((parameters.A*line.parameters.B - line.parameters.A*parameters.B)/
                    (parameters.A*line.parameters.A + parameters.B*line.parameters.B));
    }
};

}

#endif // LINE_H
