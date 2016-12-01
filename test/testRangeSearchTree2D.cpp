
#include "../src/RangeSearchTree.h"
#include "../src/Point.h"
#include "../src/RangeSearchTree2D.h"
#include <vector>
#include <set>
#include <catch.hpp>
#include <sstream>
#include "../src/util/SetToString.h"

using namespace std;


TEST_CASE("range search 2D tree simple cases") {

  vector<Point> inputVec = {
          Point(11, 2), Point(10, 3), Point(4, 10), Point(0, 0), Point(10, 10), Point(5, 5)
  };


  set<Point> input = set<Point>(inputVec.begin(), inputVec.end());

  auto pointKeyFunc = [](const Point& a) { return a; };
  RangeSearchTree2D<Point> searchTree(inputVec, pointKeyFunc);



  SECTION("works for single point") {
    prettyPrint(cout, searchTree.tree);
    const Point singlePoint = Point(1.0, 2.0);
    vector<Point> singleVec = {singlePoint};
    set<Point> expected = set<Point>(singleVec.begin(), singleVec.end());
    RangeSearchTree2D<Point> singleSearchTree(singleVec, pointKeyFunc);


    REQUIRE(singleSearchTree.search(0, 2, -1, 30) == expected);

//    REQUIRE(singleSearchTree.search(1, 2, 2, 3) == expected);
//
//    REQUIRE(singleSearchTree.search(0, 1, 2, 3) == expected);
//
//    REQUIRE(singleSearchTree.search(0, 2, 1, 3) == expected);

  }

  SECTION("works for no duplicate x or y coord") {
    vector<Point> inputVecWithNoXYdup = {
            Point(11, 2), Point(12, 3), Point(4, 123), Point(0, 0), Point(10, 10), Point(5, 5)
    };

    set<Point> expected = set<Point>();

    expected.insert(Point(12, 3));
    expected.insert(Point(11, 2));

    RangeSearchTree2D<Point> searchTreeWithNoXYdup(inputVecWithNoXYdup, pointKeyFunc);
    prettyPrint(searchTreeWithNoXYdup.tree);


    REQUIRE(searchTreeWithNoXYdup.search(11, 13, 2, 4) == expected);
  }

  SECTION("works for non duplicate x or y coord, weird case") {
    vector<Point> inputVecWithNoXYdup = {
            Point(11, 2), Point(12, 3), Point(4, 123), Point(13, 0), Point(10, 10), Point(5, 5)
    };

    set<Point> expected = set<Point>();

    expected.insert(Point(12, 3));
    expected.insert(Point(11, 2));

    RangeSearchTree2D<Point> searchTreeWithNoXYDup(inputVecWithNoXYdup, pointKeyFunc);
    prettyPrint(searchTreeWithNoXYDup.tree);


    REQUIRE(searchTreeWithNoXYDup.search(11, 13, 2, 4) == expected);
  }

  SECTION("regression, works some points") {
    vector<Point> inputVecWithNoXYdup = {
            Point(12, 2), Point(11, 3), Point(13, 4), Point(5, 70), Point(5, 100), Point(3, 101), Point(1, 70)
    };

    set<Point> expected = set<Point>();

    expected.insert(Point(13, 4));
    expected.insert(Point(11, 3));

    RangeSearchTree2D<Point> searchTreeWithXYDups(inputVecWithNoXYdup, pointKeyFunc);
    prettyPrint(searchTreeWithXYDups.tree);


    REQUIRE(searchTreeWithXYDups.search(4, 13, 2.5, 5) == expected);
  }

}
