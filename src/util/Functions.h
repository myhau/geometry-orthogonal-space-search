#ifndef GEO_PROJ_FUNCTIONS_H
#define GEO_PROJ_FUNCTIONS_H


template<typename T>
auto identity() {
  return [](const T &x) { return x; };
}


#endif //GEO_PROJ_FUNCTIONS_H
