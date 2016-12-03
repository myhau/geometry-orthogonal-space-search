#ifndef GEO_PROJ_VECTORS_H
#define GEO_PROJ_VECTORS_H


#include <vector>

using namespace std;

class Vectors {
public:

  template <typename T>
  static vector<T> concat(initializer_list<vector<T>> vectors) {
    size_t allElementsCount = 0;
    for (auto &&item : vectors) {
      allElementsCount += item.size();
    }
    vector<T> concatenated;
    concatenated.reserve(allElementsCount);

    for (auto &&vec : vectors) {
      concatenated.insert(concatenated.end(), vec.begin(), vec.end());
    }

    return concatenated;
  }
};


#endif //GEO_PROJ_VECTORS_H
