
#include "../src/RangeSearchTree.h"
#include "../src/Point.h"
#include "../src/RangeSearchTree2D.h"
#include <vector>
#include <set>
#include <catch.hpp>


using namespace std;


TEST_CASE("range search tree simple cases", "[vector]") {

  vector<Point> inputVec = {
          Point(11, 2), Point(10, 3), Point(4, 10), Point(0, 0), Point(10, 10), Point(5, 5)
  };

  set<Point> input = set<Point>(inputVec.begin(), inputVec.end());

  auto pointKeyFunc = [](const Point& a) { return a; };
  RangeSearchTree2D<Point> searchTree(inputVec, pointKeyFunc);

  SECTION("works for single point") {
    const Point singlePoint = Point(1.0, 2.0);
    std::vector<Point> singleVec = {singlePoint};
    std::set<Point> expected = set<Point>(inputVec.begin(), inputVec.end());
    RangeSearchTree2D<Point> singleSearchTree(singleVec, pointKeyFunc);

    REQUIRE(singleSearchTree.search(1, 2, 1, 2) == expected);

    REQUIRE(singleSearchTree.search(1, 2, 2, 3) == expected);

    REQUIRE(singleSearchTree.search(0, 1, 2, 3) == expected);

    REQUIRE(singleSearchTree.search(0, 2, 1, 3) == expected);

  }

}
