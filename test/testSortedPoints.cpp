
#include "catch.hpp"
#include <vector>
#include "../src/SortedPoints.h"
#include "../src/Point.h"
#include "../src/util/SetToString.h"

using namespace std;


TEST_CASE("sorted points should work") {

  vector<PointWithData<int>> points = {
          PointWithData<int>(Point<>(1, 2), 1),
          PointWithData<int>(Point<>(2, 1), 2),
          PointWithData<int>(Point<>(4, 4), 3),
          PointWithData<int>(Point<>(0, 0), 4)
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

  SECTION("test ") {
    vector<Point<double>> points2 = {point(11.0, 2.0), point(12.0, 3.0), point(4.0, 123.0), point(13.0, 0.0), point(10.0, 10.0), point(5.0, 5.0)};

    vector<PointWithData<double>> pointsWithWhateverData;

    for (auto &&item : points2) {
      pointsWithWhateverData.push_back(make_point_with_data(item, item.x));
    }

    auto sortedPoints2 = make_sorted_points(pointsWithWhateverData);

    vector<PointWithData<double>> expected = {pointsWithWhateverData[0], pointsWithWhateverData[1]};

    REQUIRE(sortedPoints2.getAllY(3, 4) == expected);

  }

}
