
#include "../src/RangeSearchTree.h"
#include "../src/Point.h"
#include "../src/RangeSearchTree2D.h"
#include <vector>
#include <set>
#include <catch.hpp>


using namespace std;

using c = function<bool(const Point&, const Point&)>;

set<Point, c> to_set(vector<Point> points, c comparator) {
  return set<Point, c>(points.begin(), points.end(), comparator);
};

TEST_CASE("range search tree simple cases", "[vector]") {

  c testPointsComparator = [](const Point& a, const Point& b) { return a.x > b.x && a.y > b.y; };


  vector<Point> inputVec = {
          Point(11, 2), Point(10, 3), Point(4, 10), Point(0, 0), Point(10, 10), Point(5, 5)
  };


  set<Point, c> input = set<Point, c>(inputVec.begin(), inputVec.end(), testPointsComparator);

  auto pointKeyFunc = [](const Point& a) { return a; };
  RangeSearchTree2D<Point> searchTree(inputVec, pointKeyFunc);

  SECTION("works for single point") {
    const Point singlePoint = Point(1.0, 2.0);
    vector<Point> singleVec = {singlePoint};
    set<Point, c> expected = set<Point, c>(inputVec.begin(), inputVec.end(), testPointsComparator);
    RangeSearchTree2D<Point> singleSearchTree(singleVec, pointKeyFunc);

    REQUIRE(to_set(singleSearchTree.search(1, 2, 1, 2), testPointsComparator) == expected);

//    REQUIRE(singleSearchTree.search(1, 2, 2, 3) == expected);
//
//    REQUIRE(singleSearchTree.search(0, 1, 2, 3) == expected);
//
//    REQUIRE(singleSearchTree.search(0, 2, 1, 3) == expected);

  }

}
