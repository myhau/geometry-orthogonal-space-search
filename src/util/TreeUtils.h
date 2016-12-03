#ifndef GEO_PROJ_TREEUTILS_H
#define GEO_PROJ_TREEUTILS_H

#include "../Node.h"
#include <utility>
#include <climits>
#include <algorithm>

using namespace std;

class TreeUtils {
public:

  template<typename T>
  static bool isWellBalanced(Node<T>* tree) {
    auto res = minMaxLeafLevel(tree, 0, INT_MAX, INT_MIN);
    return res.second - res.first < 2; // todo: check it
  }

  template<typename T>
  static pair<int, int> minMaxLeafLevel(Node<T>* n, int level, int minLevel, int maxLevel) {
    if(n == nullptr) {
      return make_pair(min(level, minLevel), max(level, maxLevel));
    }
    auto minMaxLeft = minMaxLeafLevel(n->left, level + 1, minLevel, maxLevel);
    auto minMaxRight = minMaxLeafLevel(n->right, level + 1, minLevel, maxLevel);
    return make_pair(min(minMaxLeft.first, minMaxRight.first), max(minMaxLeft.second, minMaxRight.second));
  };
};


#endif //GEO_PROJ_TREEUTILS_H
