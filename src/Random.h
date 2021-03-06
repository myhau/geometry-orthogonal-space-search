#ifndef GEO_PROJ_RANDOM_H
#define GEO_PROJ_RANDOM_H


#include "Point.h"
#include <vector>
#include <cmath>
#include <random>

using namespace std;

default_random_engine generator;

uniform_real_distribution<double> angleDistribution(0, 2 * M_PI);

class Random {
public:

  static auto randomPoints(const Rect<>& insideRect, int count) {
    uniform_real_distribution<double> xDistribution(insideRect.xFrom, insideRect.xTo);
    uniform_real_distribution<double> yDistribution(insideRect.yFrom, insideRect.yTo);

    vector<Point<>> points;

    for (int i = 0; i < count; ++i) {
      points.push_back(point(xDistribution(generator), yDistribution(generator)));
    }
    return points;
  }

  static auto randomPoint(const Rect<>& insideRect) {
    return randomPoints(insideRect, 1)[0];
  }

  static auto randomRect(const Rect<>& insideRect) {
    return make_rect(randomPoint(insideRect), randomPoint(insideRect));
  }

  inline static auto randomPointsOnCircle(Point<> center, double r, int count) {

    vector<Point<>> out;
    for (int i = 0; i < count; ++i) {
      double angle = angleDistribution(generator);

      double x = r * cos(angle) + center.x;
      double y = r * sin(angle) + center.y;

      out.push_back(Point<>(x, y));
    }

    return out;
  }

  inline static auto randomPointsInCircle(Point<> center, double maxR, int count) {
    uniform_real_distribution<double> rDistribution(0, maxR);

    vector<Point<>> out;
    for (int i = 0; i < count; ++i) {
      double angle = angleDistribution(generator);
      double r = rDistribution(generator);

      double x = r * cos(angle) + center.x;
      double y = r * sin(angle) + center.y;

      out.push_back(point(x, y));
    }

    return out;
  }

  inline static auto randomPointsInRect(double minX, double maxX, double minY, double maxY,
                                                 int count) {
    uniform_real_distribution<double> xDistribution(minX, maxX);
    uniform_real_distribution<double> yDistribution(minY, maxY);

    vector<Point<>> out;
    for (int i = 0; i < count; ++i) {

      out.push_back(
              point(xDistribution(generator), yDistribution(generator))
      );

    }

    return out;
  }

  inline static auto randomPointsOnRect(double minX, double maxX, double minY, double maxY,
                                                 double count) {

    double r[2][2] = {{minX, maxX}, {minY, maxY}};

    int whichCoordIsConst[] = {0, 1};
    discrete_distribution<int> chooseFrom0To1({1, 1});

    vector<Point<>> out;
    for (int i = 0; i < count; ++i) {

      int c1 = whichCoordIsConst[chooseFrom0To1(generator)];
      int c2 = 1 - c1;

      uniform_real_distribution<double> distr(r[c2][0], r[c2][1]);

      double coords[2];

      coords[c1] = r[c1][chooseFrom0To1(generator)];
      coords[c2] = distr(generator);

      out.push_back(Point<>(coords));
    }

    return out;
  }
};


#endif //GEO_PROJ_RANDOM_H
