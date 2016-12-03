
#include "../src/RangeSearchTree.h"
#include "../src/Point.h"
#include "../src/RangeSearchTree2D.h"
#include <vector>
#include <set>
#include <catch.hpp>
#include <sstream>
#include "../src/util/SetToString.h"
#include "../src/Random.h"
#include "../src/util/Vectors.h"
#include "../src/util/BitmapToProblem.h"
#include "../src/util/TreeUtils.h"
#include "../src/SimpleSearchAlgo2D.h"


using namespace std;


TEST_CASE("range search 2D tree simple cases") {

  SECTION("works for single point") {
    const Point singlePoint = Point(1.0, 2.0);
    vector<Point> singleVec = {singlePoint};
    set<Point> expected = set<Point>(singleVec.begin(), singleVec.end());
    auto singleSearchTree = searchTreeOfPoints(singleVec);


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

    auto searchTreeWithNoXYdup = searchTreeOfPoints(inputVecWithNoXYdup);
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

    auto searchTreeWithNoXYDup = searchTreeOfPoints(inputVecWithNoXYdup);
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

    auto searchTreeForSomePoints = searchTreeOfPoints(inputVecWithNoXYdup);
    prettyPrint(searchTreeForSomePoints.tree);


    REQUIRE(searchTreeForSomePoints.search(4, 13, 2.5, 5) == expected);
  }

  SECTION("works for points on circle, should not find any") {
    vector<Point> points = Random::randomPointsOnCircle(Point(0, 0), 10, 1000);

    set<Point> expected = set<Point>();

    auto searchTreeOnCircle = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnCircle.search(-5, 5, -5, 3) == expected);
  }

  SECTION("works for points on circle, should find all points") {
    vector<Point> points = Random::randomPointsOnCircle(Point(0, 0), 10, 1000);

    set<Point> expected = set<Point>(points.begin(), points.end());

    auto searchTreeOnCircle = searchTreeOfPoints(points);


    REQUIRE(searchTreeOnCircle.search(-11, 10.5, -25, 11.9) == expected);
  }

  SECTION("works for multiple circles with multiple densities, captures only whole two of em") {

    auto onCircle1Center = Point(-10, 10);
    vector<Point> onCircle1 = Random::randomPointsOnCircle(onCircle1Center, 3, 100);

    auto circle2Center = Point(-90, 90);
    auto circle2R = 4;
    vector<Point> insideCircle2 = Random::randomPointsInCircle(circle2Center, circle2R, 100);

    vector<Point> insideCircle3 = Random::randomPointsInCircle(Point(-100, 100), 5, 200);

    auto circle4Center = Point(10, 5);
    auto circle4R = 3;
    vector<Point> insideCircle4 = Random::randomPointsInCircle(circle4Center, circle4R, 100);


    vector<Point> allPoints = Vectors::concat({onCircle1, insideCircle2, insideCircle3, insideCircle4});

    auto searchTreeOfCircles = searchTreeOfPoints(allPoints);


    SECTION("should not find any element inside circle that has only points on circle") {
      auto slightVectorFromCenter = Point(0.001, -0.001);
      REQUIRE(
              searchTreeOfCircles.search(Rect(onCircle1Center + slightVectorFromCenter,
                                              onCircle1Center + slightVectorFromCenter))
              ==
              set<Point>()
      );
    }

    SECTION("should find only two circle chunks") {
      auto beforeAllCircle2Points = circle2Center - Point(circle2R + 0.01, circle2R - 0.01);
      auto afterAllCircle4Points = circle4Center + Point(circle4R + 0.01, circle4R + 0.01);

      auto expectedVector = Vectors::concat({insideCircle2, insideCircle4});
      auto expected = set<Point>(expectedVector.begin(), expectedVector.end());

      REQUIRE(
              searchTreeOfCircles.search(Rect(beforeAllCircle2Points, afterAllCircle4Points))
              ==
              expected
      );
    }

  }

  SECTION("check if huge search tree is well balanced (should be because of building it from sorted array basing on median)") {
    vector<Point> insideCircle3 = Random::randomPointsInCircle(Point(-100, 100), 5, 2000);

    auto circle4Center = Point(10, 5);
    auto circle4R = 3;
    vector<Point> insideCircle4 = Random::randomPointsInCircle(circle4Center, circle4R, 1000);


    vector<Point> allPoints = Vectors::concat({insideCircle3, insideCircle4});

    auto searchTreeOfCircles = searchTreeOfPoints(allPoints);

    REQUIRE(TreeUtils::isWellBalanced(searchTreeOfCircles.tree));
  }

  SECTION("works for some hand defined cases") {
    auto problemAndPoints = BitmapToProblem::bitmapToProblemInputs(
            {
                    {'x', 'x', 'x'},
                    {'-', '-', '-'},
                    {'|', 'x', '|'},
                    {'_', '_', '_'}
            }
    );

    Rect inputRect = problemAndPoints.first;
    vector<Point> points = problemAndPoints.second;

    auto searchTree1 = searchTreeOfPoints(points);

    set<Point> expected;
    expected.insert(Point(1, -2));

    REQUIRE(
            searchTree1.search(inputRect)
            ==
            expected
    );

  }


  SECTION("should find one point") {
    vector<Point> points = {Point(0, 0), Point(1, 1)};

    set<Point> expected;
    expected.insert(Point(0, 0));

    auto searchTree1 = searchTreeOfPoints(points);

    REQUIRE(
            searchTree1.search(0, 0, 0, 0)
            ==
            expected
    );
  }

  SECTION("works for a lot of duplicates when should be empty match") {
    vector<Point> points = Random::randomPointsOnRect(10.0, 20.0, 10.0, 50.0, 1000);

    set<Point> expected = set<Point>();

    auto searchTreeOnRect = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnRect.search(11, 19, 11, 49) == expected);
  }


  SECTION("works for a lot of duplicates when should be all-in match") {
    vector<Point> points = Random::randomPointsOnRect(10.0, 20.0, 10.0, 50.0, 1000);

    set<Point> expected = set<Point>(points.begin(), points.end());

    auto searchTreeOnRect = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnRect.search(9.0, 21.0, 9.0, 55.0) == expected);
  }


  SECTION("there should be no difference if searched area is big or small, if it captures all points, it should return all points, no more, no less") {
    vector<Point> points = Random::randomPointsOnRect(10.0, 20.0, 10.0, 50.0, 1000);

    set<Point> expected = set<Point>(points.begin(), points.end());

    auto searchTreeOnRect = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnRect.search(9.0, 21.0, 9.0, 55.0) == expected);
    REQUIRE(searchTreeOnRect.search(-10000.0, 12312.0, -1000.00, 1000.0) == expected);
  }

}


TEST_CASE("test a lot of random cases by comparing results with simple algorithm that 100% works") {

  auto N = 10000;
  auto EACH_COUNT_MULT = 1;

  for (int i = 0; N > i; i++) {
    auto pointCount = EACH_COUNT_MULT * i;
    stringstream sectionName;
    sectionName << "should work for i: " << i << " point count: " << pointCount;
    SECTION(sectionName.str()) {
      auto rect = Rect(Point(-100, -100), Point(100, 100));

      auto points = Random::randomPoints(rect, pointCount);
      auto searchRect = Random::randomRect(rect);

      INFO("max rect is " << rect);
      INFO("search rect is " << searchRect);
      INFO("input points are " << points);

      auto simpleSearch = SimpleSearchAlgo2D(points);
      auto treeUnderTest = searchTreeOfPoints(points);

      auto expectedPoints = simpleSearch.search(searchRect);
      auto actualPoints = treeUnderTest.search(searchRect);

      REQUIRE(expectedPoints == actualPoints);
    }
  }

}
