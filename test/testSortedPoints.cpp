
#include "catch.hpp"
#include <vector>
#include "../src/SortedPoints.h"
#include "../src/Point.h"

using namespace std;


TEST_CASE("sorted points should work") {

  vector<PointWithData<int>> points = {
          PointWithData<int>(Point(1, 2), 1),
          PointWithData<int>(Point(2, 1), 2),
          PointWithData<int>(Point(4, 4), 3),
          PointWithData<int>(Point(0, 0), 4)
  };

  SortedPoints<int> sortedPoints(points);

  SECTION("should keep points sorted by x") {

    vector<PointWithData<int>> actual;
    for (size_t i = 0; i < sortedPoints.size(); ++i) {
      actual.push_back(sortedPoints.getX(i));
    }
    vector<PointWithData<int>> expected = {points[3], points[0], points[1], points[2]};

    REQUIRE(expected == actual);
  }

  SECTION("should return correct sorted y set filtered by x edge values") {

    vector<PointWithData<int>> expected = {points[3], points[1], points[0], points[2]};

    vector<PointWithData<int>> actual = sortedPoints.getAllY(0, 3);

    REQUIRE(expected == actual);
  }

}
