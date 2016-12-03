#ifndef GEO_PROJ_BITMAPTOPOINTS_H
#define GEO_PROJ_BITMAPTOPOINTS_H


#include "../Point.h"
#include "../Rect.h"
#include <vector>
#include <algorithm>

using namespace std;

class BitmapToProblem {
public:
  /**
   * [x] is point
   * [ ] is nothing
   * [-] and [_] and [|] are
   * [*] is comabination of divider [-|_] and point [x]
   *
   * it does not check if input is valid
   */
  static pair<Rect, vector<Point>> bitmapToProblemInputs(vector<vector<char>> bitmap, double multiplier = 1.0) {
    double minX = 0, maxX = 0, minY = 0, maxY = 0;

    vector<Point> out;
    for (int i = 0; i < bitmap.size(); ++i) {
      for (int j = 0; j < bitmap[i].size(); ++j) {
        double dI = (double) i;
        switch (bitmap[i][j]) {
          case 'x':
            out.push_back(Point(i, -j));
            break;
          case '*':
            out.push_back(Point(i, -j));
          case '-':
          case '_':
          case '|':
            minX = min(minX, dI);
            maxX = max(maxX, dI);
            minY = min(minY, dI);
            maxY = max(maxY, dI);
            break;

          default:
            break;
        }
      }
    }

    minX *= multiplier;
    maxX *= multiplier;
    minY *= multiplier;
    maxY *= multiplier;

    vector<Point> outMultiplied(out.size());
    transform(out.begin(), out.end(), outMultiplied.begin(),
              [&multiplier](const Point &p) { return p * multiplier; });

    return make_pair(Rect(minX, maxX, minY, maxY), outMultiplied);
  }
};


#endif //GEO_PROJ_BITMAPTOPOINTS_H
