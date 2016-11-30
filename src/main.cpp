#include <iostream>
#include <catch.hpp>
#include "RangeSearchTree2D.h"
#include "util/Functions.h"

int main() {
  vector<Point> points = {Point(1, 0), Point(0,1)};

  auto searchTree = RangeSearchTree2D(points, identity());



  return 0;
}