#ifndef GEO_PROJ_SORTEDPOINTS_H
#define GEO_PROJ_SORTEDPOINTS_H

#include <vector>
#include <algorithm>
#include <functional>
#include "Point.h"
#include <numeric>
#include <set>

using namespace std;

template<typename T>
class SortedPoints {
private:
  vector<size_t> xSortedIndexes;
  vector<size_t> ySortedIndexes;
  vector<size_t> reverseXIndex;
  vector<PointWithData<T>> points;

  function<int(const size_t &, const size_t &)>
  pointComparator(function<double(const Point &)> f) {
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

  SortedPoints(const vector<PointWithData<T>> &points) : xSortedIndexes(points.size()),
                                                         ySortedIndexes(points.size()),
                                                         points(points) {
    iota(xSortedIndexes.begin(), xSortedIndexes.end(), 0);
    iota(ySortedIndexes.begin(), ySortedIndexes.end(), 0);

    std::sort(xSortedIndexes.begin(), xSortedIndexes.end(),
              pointComparator([](const Point &p) { return p.x; }));

    std::sort(ySortedIndexes.begin(), ySortedIndexes.end(),
              pointComparator([](const Point &p) { return p.y; }));


    reverseXIndex = transposeVector(xSortedIndexes);
  }



  size_t size() {
    return points.size();
  }

  const PointWithData<T> &getX(size_t i) const {
    return points[xSortedIndexes[i]];
  }

  vector<PointWithData<T>> getAllY(size_t fromX, size_t toX) const {

    vector<size_t> subIndexes(xSortedIndexes.begin() + fromX, xSortedIndexes.begin() + toX + 1);

    set<size_t> mappedToYIndexes;

    for (auto& index: subIndexes) {
      mappedToYIndexes.insert(ySortedIndexes[reverseXIndex[index]]);
    }

    vector<PointWithData<T>> out;

    for (auto yIndex : ySortedIndexes) {
      if(mappedToYIndexes.find(yIndex) != mappedToYIndexes.end()) {
        out.push_back(points[yIndex]);
      }
    }

    return out;

  }

};


#endif //GEO_PROJ_SORTEDPOINTS_H
