#ifndef GEO_PROJ_SETTOSTRING_H
#define GEO_PROJ_SETTOSTRING_H

#include <iostream>
#include <set>
#include "../Point.h"
#include "type_traits"

using namespace std;

ostream& operator << ( ostream& os, set<Point> const& values );
ostream& operator << ( ostream& os, set<double> const& values );

template<typename T>
ostream& operator << ( ostream& os, set<PointWithData<T>> const& values );


#endif //GEO_PROJ_SETTOSTRING_H
