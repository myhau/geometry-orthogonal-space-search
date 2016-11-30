#include "SetToString.h"


// TODO: use some c++ template mechanism to deduplicate that


ostream& operator << ( ostream& os, set<Point> const& values ) {
  os << "{";
  for (auto &&item :values) {
    os << item << ",";
  }
  os << "}";

  return os;
}

ostream& operator << ( ostream& os, set<double> const& values ) {
  os << "{";
  for (auto &&item :values) {
    os << item << ",";
  }
  os << "}";

  return os;
}

template<typename T>
ostream& operator << ( ostream& os, set<PointWithData<T>> const& values ) {
  os << "{";
  for (auto &&item :values) {
    os << item << ",";
  }
  os << "}";

  return os;
}