#ifndef GEO_PROJ_BITMAPTOPOINTS_H
#define GEO_PROJ_BITMAPTOPOINTS_H


#include "../Point.h"
#include "../Rect.h"
#include <vector>
#include <algorithm>
#include <limits>

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

  static vector<Point> bitmapToPoints(vector<string> bitmap, double multiplier = 1.0) {
    return bitmapToProblemInputs(bitmap, multiplier).second;
  }

  static pair<Rect, vector<Point>> bitmapToProblemInputs(vector<string> bitmap, double multiplier = 1.0) {
    vector<vector<char>> out(bitmap.size());
    transform(bitmap.begin(), bitmap.end(), out.begin(), [](const string& line){return vector<char>(line.begin(), line.end());});
    return bitmapToProblemInputs(out, multiplier);
  }
private:
  static vector<Point> bitmapToPoints(vector<vector<char>> bitmap, double multiplier = 1.0) {
    return bitmapToProblemInputs(bitmap, multiplier).second;
  }
  static pair<Rect, vector<Point>> bitmapToProblemInputs(vector<vector<char>> bitmap, double multiplier = 1.0) {
    double inf = numeric_limits<double>::infinity();
    double minX = inf, maxX = -inf, minY = inf, maxY = -inf;

    vector<Point> out;
    for (int i = 0; i < bitmap.size(); ++i) {
      for (int j = 0; j < bitmap[i].size(); ++j) {
        double dy = -i;
        double dx = j;
        switch (bitmap[i][j]) {
          case 'x':
            out.push_back(Point(dx, dy));
            break;
          case '*':
            out.push_back(Point(dx, dy));
          case '-':
          case '_':
          case '|':
            minX = min(minX, dx);
            maxX = max(maxX, dx);
            minY = min(minY, dy);
            maxY = max(maxY, dy);
            break;
          case ' ':
          case '.':
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
