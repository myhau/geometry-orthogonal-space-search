#ifndef GEO_PROJ_TREEUTILS_H
#define GEO_PROJ_TREEUTILS_H

#include "../Node.h"
#include <utility>
#include <limits>
#include <algorithm>

using namespace std;

class TreeUtils {
public:

  template<typename T, typename C>
  static bool isWellBalanced(Node<T, C>* tree) {
    auto res = minMaxLeafLevel(tree, 0, numeric_limits<int>::max(), numeric_limits<int>::min());
    return res.second - res.first < 2;
  }

  template<typename T, typename C>
  static pair<int, int> minMaxLeafLevel(Node<T, C>* n, int level, int minLevel, int maxLevel) {
    if(n == nullptr) {
      return make_pair(min(level, minLevel), max(level, maxLevel));
    }
    auto minMaxLeft = minMaxLeafLevel(n->left, level + 1, minLevel, maxLevel);
    auto minMaxRight = minMaxLeafLevel(n->right, level + 1, minLevel, maxLevel);
    return make_pair(min(minMaxLeft.first, minMaxRight.first), max(minMaxLeft.second, minMaxRight.second));
  };
};


#endif //GEO_PROJ_TREEUTILS_H
