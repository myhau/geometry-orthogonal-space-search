#define CATCH_CONFIG_MAIN


#include "catch.hpp"
#include "../src/RangeSearchTree.h"
#include <vector>


using namespace std;


TEST_CASE("range search tree simple cases", "[vector]") {

  std::vector<double> inputVec = {1, 2, 3, 4, 8};
  std::set<double> input = set<double>(inputVec.begin(), inputVec.end());

  auto doubleKeyFunc = [](double a) { return a; };
  RangeSearchTree<double> searchTree(inputVec, doubleKeyFunc);

  SECTION("works for single point") {
    std::vector<double> singleVec = {1};
    RangeSearchTree<double> singleSearchTree(singleVec, doubleKeyFunc);

    set<double> expected;

    expected = {1};
    REQUIRE(singleSearchTree.search(1, 1) == expected);

    expected = {1};
    REQUIRE(singleSearchTree.search(0, 1) == expected);

    expected = {1};
    REQUIRE(singleSearchTree.search(1, 2) == expected);

    expected = {1};
    REQUIRE(singleSearchTree.search(0, 2) == expected);

    expected = {};
    REQUIRE(singleSearchTree.search(2, 2) == expected);

    expected = {};
    REQUIRE(singleSearchTree.search(-1, 0) == expected);

    expected = {};
    REQUIRE(singleSearchTree.search(1.5, 2) == expected);
  }

  SECTION("finding non existing point should return nothing") {
    auto found = searchTree.search(10, 20);
    REQUIRE(found == set<double>());
  }
  SECTION("should find single at the end point") {
    auto found = searchTree.search(3.5, 4.5);

    set<double> expected{4};
    REQUIRE(found == expected);
  }

  SECTION("should find one point at the beggining") {
    auto found = searchTree.search(0.9, 1.9);

    set<double> expected{1};
    REQUIRE(found == expected);
  }

  SECTION("should find two points") {
    auto found = searchTree.search(2.5, 4.5);

    set<double> expected{3, 4};
    REQUIRE(found == expected);
  }

  SECTION("should find no points because too small segment") {
    auto found = searchTree.search(2.1, 2.9);

    set<double> expected{};
    REQUIRE(found == expected);
  }

  SECTION("should find all elements") {
    auto found = searchTree.search(-100.0, 100.0);

    REQUIRE(input == found);
  }

  SECTION("should not be right side inclusive when finding all") {
    auto found = searchTree.search(1, 4);

    set<double> expected{1, 2, 3, 4};
    REQUIRE(found == expected);
  }

  SECTION("inclusive edge cases") {
    set<double> expected;
    set<double> found;


    found = searchTree.search(1, 1);
    expected = {1};
    REQUIRE(found == expected);

    found = searchTree.search(0, 1);
    expected = {1};
    REQUIRE(found == expected);

    found = searchTree.search(1, 1.5);
    expected = {1};
    REQUIRE(found == expected);

    found = searchTree.search(4, 4);
    expected = {4};
    REQUIRE(found == expected);

    found = searchTree.search(3.5, 4);
    expected = {4};
    REQUIRE(found == expected);

    found = searchTree.search(4, 4.5);
    expected = {4};
    REQUIRE(found == expected);


    found = searchTree.search(8, 8.1);
    expected = {8};
    REQUIRE(found == expected);

    found = searchTree.search(3, 3);
    expected = {3};
    REQUIRE(found == expected);

    found = searchTree.search(2.9, 3.1);
    expected = {3};
    REQUIRE(found == expected);


    found = searchTree.search(3, 3.1);
    expected = {3};
    REQUIRE(found == expected);

  }

  SECTION("should not allow bad input") {
    REQUIRE_THROWS(searchTree.search(10, 9));
  }


}
