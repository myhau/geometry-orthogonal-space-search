#ifndef GEO_PROJ_SORTEDPOINTS_H
#define GEO_PROJ_SORTEDPOINTS_H

#include <vector>
#include <algorithm>
#include <functional>
#include "Point.h"
#include <numeric>
#include <set>

using namespace std;

template<typename T, typename C = double>
class SortedPoints {
private:
  vector<size_t> xSortedIndexes;
  vector<size_t> ySortedIndexes;
  vector<size_t> reverseYIndex;
  vector<PointWithData<T, C>> points;

  function<int(const size_t &, const size_t &)>
  pointComparator(function<C(const Point<C> &)> f) {
    return [this, f](const size_t &i1, const size_t &i2) {
      return f(points[i2].point) > f(points[i1].point);
    };
  }

  vector<size_t> transposeVector(vector<size_t> v) {
    vector<size_t> out(v.size(), 0);
    for (size_t i = 0; i < v.size(); ++i) {
      out[v[i]] = i;
    }
    return out;
  }

public:

  SortedPoints(const vector<PointWithData<T, C>> &points) : xSortedIndexes(points.size()),
                                                         ySortedIndexes(points.size()),
                                                         points(points) {
    iota(xSortedIndexes.begin(), xSortedIndexes.end(), 0);
    iota(ySortedIndexes.begin(), ySortedIndexes.end(), 0);

    std::sort(xSortedIndexes.begin(), xSortedIndexes.end(),
              pointComparator([](const Point<C> &p) { return p.x; }));

    std::sort(ySortedIndexes.begin(), ySortedIndexes.end(),
              pointComparator([](const Point<C> &p) { return p.y; }));


    reverseYIndex = transposeVector(ySortedIndexes);
  }



  size_t size() {
    return points.size();
  }

  const PointWithData<T, C> &getX(size_t i) const {
    return points[xSortedIndexes[i]];
  }

  vector<PointWithData<T, C>> getAllY(size_t fromX, size_t toX) const {

    vector<size_t> subIndexes(xSortedIndexes.begin() + fromX, xSortedIndexes.begin() + toX + 1);

    set<size_t> mappedToYIndexes;

    for (auto& index: subIndexes) {
      mappedToYIndexes.insert(ySortedIndexes[reverseYIndex[index]]);
    }

    vector<PointWithData<T, C>> out;

    for (auto yIndex : ySortedIndexes) {
      if(mappedToYIndexes.find(yIndex) != mappedToYIndexes.end()) {
        out.push_back(points[yIndex]);
      }
    }

    return out;

  }

};


template<typename T, typename C>
SortedPoints<T, C> make_sorted_points(const vector<PointWithData<T, C>> &points) {
  return SortedPoints<T, C>(points);
}

#endif //GEO_PROJ_SORTEDPOINTS_H
