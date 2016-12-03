#ifndef GEO_PROJ_SETTOSTRING_H
#define GEO_PROJ_SETTOSTRING_H

#include <iostream>
#include <set>
#include <vector>
#include "../Point.h"
#include "type_traits"

using namespace std;


ostream& operator << ( ostream& os, vector<Point<>> const& values );
ostream& operator << ( ostream& os, set<Point<>> const& values );

ostream& operator << ( ostream& os, set<double> const& values );
ostream& operator << ( ostream& os, set<ComparableTuple> const& values );


template<typename T, typename C>
ostream& operator << ( ostream& os, set<PointWithData<T, C>> const& values );


#endif //GEO_PROJ_SETTOSTRING_H
