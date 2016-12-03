#include "SetToString.h"


// TODO: use some c++ template mechanism to deduplicate that

template<typename T>
ostream& commonOStream(ostream& os, T values) {
  os << "{";
  for (auto &&item :values) {
    os << item << ",";
  }
  os << "}";

  return os;
}

template <typename C>
ostream& operator << ( ostream& os, vector<Point<C>> const& values ) {
  return commonOStream(os, values);
}

ostream& operator << ( ostream& os, vector<Point<>> const& values ) {
  return commonOStream(os, values);
}
ostream& operator << ( ostream& os, set<Point<>> const& values ) {
  return commonOStream(os, values);
}

template <typename C>
ostream& operator << ( ostream& os, set<Point<C>> const& values ) {
  return commonOStream(os, values);
}

ostream& operator << ( ostream& os, set<double> const& values ) {
  return commonOStream(os, values);
}


ostream& operator << ( ostream& os, set<ComparableTuple> const& values ) {
  return commonOStream(os, values);
}

template<typename T, typename C>
ostream& operator << ( ostream& os, set<PointWithData<T, C>> const& values ) {
  return commonOStream(os, values);
}