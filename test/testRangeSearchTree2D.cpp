
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
    auto singlePoint = point(1.0, 2.0);
    vector<Point<>> singleVec = {singlePoint};
    auto expected = set<Point<>>(singleVec.begin(), singleVec.end());
    auto singleSearchTree = searchTreeOfPoints(singleVec);


    REQUIRE(singleSearchTree.search(0.0, 2.0, -1.0, 30.0) == expected);

//    REQUIRE(singleSearchTree.search(1, 2, 2, 3) == expected);
//
//    REQUIRE(singleSearchTree.search(0, 1, 2, 3) == expected);
//
//    REQUIRE(singleSearchTree.search(0, 2, 1, 3) == expected);

  }

  SECTION("works for no duplicate x or y coord") {
    vector<Point<>> inputVecWithNoXYdup = {
            point(11.0, 2.0), point(12.0, 3.0), point(4.0, 123.0), point(0.0, 0.0), point(10.0, 10.0), point(5.0, 5.0)
    };

    auto expected = set<Point<>>();

    expected.insert(point(12.0, 3.0));
    expected.insert(point(11.0, 2.0));

    auto searchTreeWithNoXYdup = searchTreeOfPoints(inputVecWithNoXYdup);
    prettyPrint(searchTreeWithNoXYdup.tree);


    REQUIRE(searchTreeWithNoXYdup.search(11.0, 13.0, 2.0, 4.0) == expected);
  }

  SECTION("works for non duplicate x or y coord, weird case") {
    vector<Point<>> inputVecWithNoXYdup = {
            point(11.0, 2.0), point(12.0, 3.0), point(4.0, 123.0), point(13.0, 0.0), point(10.0, 10.0), point(5.0, 5.0)
    };

    auto expected = set<Point<>>();

    expected.insert(point(12.0, 3.0));
    expected.insert(point(11.0, 2.0));

    auto searchTreeWithNoXYDup = searchTreeOfPoints(inputVecWithNoXYdup);
    prettyPrint(searchTreeWithNoXYDup.tree);


    REQUIRE(searchTreeWithNoXYDup.search(11.0, 13.0, 2.0, 4.0) == expected);
  }

  SECTION("regression, works some points") {
    vector<Point<>> inputVecWithNoXYdup = {
            point(12.0, 2.0), point(11.0, 3.0), point(13.0, 4.0), point(5.0, 70.0), point(6.0, 100.0), point(3.0, 101.0), point(1.0, 75.0)
    };

    auto expected = set<Point<>>();

    expected.insert(point(13.0, 4.0));
    expected.insert(point(11.0, 3.0));

    auto searchTreeForSomePoints = searchTreeOfPoints(inputVecWithNoXYdup);
    prettyPrint(searchTreeForSomePoints.tree);


    REQUIRE(searchTreeForSomePoints.search(4.0, 13.0, 2.5, 5.0) == expected);
  }

  SECTION("works for points on circle, should not find any") {
    vector<Point<>> points = Random::randomPointsOnCircle(point(0.0, 0.0), 10.0, 1000);

    auto expected = set<Point<>>();

    auto searchTreeOnCircle = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnCircle.search(-5.0, 5.0, -5.0, 3.0) == expected);
  }

  SECTION("works for points on circle, should find all points") {
    vector<Point<>> points = Random::randomPointsOnCircle(point(0.0, 0.0), 10.0, 1000);

    auto expected = set<Point<>>(points.begin(), points.end());

    auto searchTreeOnCircle = searchTreeOfPoints(points);


    REQUIRE(searchTreeOnCircle.search(-11.0, 10.5, -25.0, 11.9) == expected);
  }

  SECTION("works for multiple circles with multiple densities, captures only whole two of em") {

    auto onCircle1Center = point(-1000.0, 10.0);
    auto onCircle1 = Random::randomPointsOnCircle(onCircle1Center, 3.0, 100);

    auto circle2Center = point(-90.0, 90.0);
    auto circle2R = 4;
    auto insideCircle2 = Random::randomPointsInCircle(circle2Center, circle2R, 100);

    auto insideCircle3 = Random::randomPointsInCircle(point(-100.0, 100.0), 5, 200);

    auto circle4Center = point(10.0, 5.0);
    auto circle4R = 3;
    auto insideCircle4 = Random::randomPointsInCircle(circle4Center, circle4R, 100);


    auto allPoints = Vectors::concat({onCircle1, insideCircle2, insideCircle3, insideCircle4});

    auto searchTreeOfCircles = searchTreeOfPoints(allPoints);


    SECTION("should not find any element inside circle that has only points on circle") {
      auto slightVectorFromCenter = point(0.001, -0.001);
      REQUIRE(
              searchTreeOfCircles.search(make_rect(onCircle1Center + slightVectorFromCenter,
                                              onCircle1Center + slightVectorFromCenter))
              ==
              set<Point<>>()
      );
    }

    SECTION("should find only two circle chunks") {
      auto beforeAllCircle2Points = circle2Center - point(circle2R + 0.01, -(circle2R + 0.01));
      auto afterAllCircle4Points = circle4Center + point(circle4R + 0.01, -(circle4R + 0.01));

      auto expectedVector = Vectors::concat({insideCircle2, insideCircle4});
      auto expected = set<Point<>>(expectedVector.begin(), expectedVector.end());

      REQUIRE(
              searchTreeOfCircles.search(make_rect(beforeAllCircle2Points, afterAllCircle4Points))
              ==
              expected
      );
    }

  }

  SECTION("check if huge search tree is well balanced (should be because of building it from sorted array basing on median)") {
    auto insideCircle3 = Random::randomPointsInCircle(point(-100.0, 100.0), 5.0, 2000);

    auto circle4Center = point(10.0, 5.0);
    auto circle4R = 3.0;
    auto insideCircle4 = Random::randomPointsInCircle(circle4Center, circle4R, 1000);


    auto allPoints = Vectors::concat({insideCircle3, insideCircle4});

    auto searchTreeOfCircles = searchTreeOfPoints(allPoints);

    REQUIRE(TreeUtils::isWellBalanced(searchTreeOfCircles.tree));
  }

  SECTION("works for some hand defined cases") {

    auto problemAndPoints = BitmapToProblem::bitmapToProblemInputs(
            {
                    "x..",
                    "___",
                    "|x|",
                    "___"
            }
    );

    auto expectedPoints = BitmapToProblem::bitmapToPoints(
            {
                    "...",
                    "...",
                    ".x.",
                    "..."
            }
    );

    auto inputRect = problemAndPoints.first;
    auto points = problemAndPoints.second;

    auto searchTree1 = searchTreeOfPoints(points);

    set<Point<>> expected(expectedPoints.begin(), expectedPoints.end());

    REQUIRE(
            searchTree1.search(inputRect)
            ==
            expected
    );

  }


  SECTION("should find one point") {
    vector<Point<>> points = {point(0.0, 0.0), point(1.0, 1.0)};

    set<Point<>> expected;
    expected.insert(point(0.0, 0.0));

    auto searchTree1 = searchTreeOfPoints(points);

    REQUIRE(
            searchTree1.search(0.0, 0.0, 0.0, 0.0)
            ==
            expected
    );
  }

  SECTION("works for a lot of duplicates when should be empty match") {
    auto points = Random::randomPointsOnRect(10.0, 20.0, 10.0, 50.0, 1000);

    auto expected = set<Point<>>();

    auto searchTreeOnRect = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnRect.search(11.0, 19.0, 11.0, 49.0) == expected);
  }


  SECTION("works for a lot of duplicates when should be all-in match") {
    auto points = Random::randomPointsOnRect(10.0, 20.0, 10.0, 50.0, 1000);

    auto expected = set<Point<>>(points.begin(), points.end());

    auto searchTreeOnRect = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnRect.search(9.0, 21.0, 9.0, 55.0) == expected);
  }


  SECTION("duplicates, there should be no difference if searched area is big or small, if it captures all points, it should return all points, no more, no less") {
    auto points = Random::randomPointsOnRect(10.0, 20.0, 10.0, 50.0, 1000);

    auto expected = set<Point<>>(points.begin(), points.end());

    auto searchTreeOnRect = searchTreeOfPoints(points);

    REQUIRE(searchTreeOnRect.search(9.0, 21.0, 9.0, 55.0) == expected);
    REQUIRE(searchTreeOnRect.search(-10000.0, 12312.0, -1000.00, 1000.0) == expected);
  }

}

TEST_CASE("regression of bug from random cases") {
  auto xFrom = -80.6745;
  auto xTo = -32.4586;
  auto yFrom = -86.7603;
  auto yTo = 20.6691;

  vector<Point<>> inputPoints = {
          point(-63.3324, 64.593),
          point(54.527, 66.7813), point(-92.5699, -66.5828), point(-34.4144, -26.0688),
          point(50.4202, -0.227207), point(-81.2191, -29.9168), point(-11.1975, -84.5985),
          point(-43.7512, 48.5778)
  };

  set<Point<>> expected;
  expected.insert(point(-34.4144, -26.0688));

  auto treeUnderTest = searchTreeOfPoints(inputPoints);

  REQUIRE(treeUnderTest.search(xFrom, xTo, yFrom, yTo) == expected);
}


TEST_CASE("test a lot of random cases by comparing results with simple algorithm that 100% works") {

  auto N = 500;
  auto EACH_COUNT_MULT = 1;

  for (int i = 0; N > i; i++) {
    auto pointCount = EACH_COUNT_MULT * i;
    stringstream sectionName;
    sectionName << "should work for i: " << i << " point count: " << pointCount;
    SECTION(sectionName.str()) {
      auto rect = make_rect(point(-100.0, -100.0), point(100.0, 100.0));

      auto points = Random::randomPoints(rect, pointCount);
      auto searchRect = Random::randomRect(rect);

      INFO("max rect is " << rect);
      INFO("search rect is " << searchRect);
      INFO("input points are " << points);

      auto simpleSearch = SimpleSearchAlgo2D<>(points);
      auto treeUnderTest = searchTreeOfPoints(points);

      auto expectedPoints = simpleSearch.search(searchRect);
      auto actualPoints = treeUnderTest.search(searchRect);

      REQUIRE(expectedPoints == actualPoints);
    }
  }

}


