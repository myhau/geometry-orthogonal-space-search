#ifndef GEO_PROJ_PRINTUTIL_H
#define GEO_PROJ_PRINTUTIL_H

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
void printVector(vector<T> vec) {
  for(auto el: vec) {
    cout << el << ",";
  }
  cout << endl;
}


#endif //GEO_PROJ_PRINTUTIL_H
