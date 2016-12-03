#ifndef GEO_PROJ_PRETTYPRINTNODE_H
#define GEO_PROJ_PRETTYPRINTNODE_H

#include <cstdio>
#include <iostream>
#include "../Node.h"
using namespace std;

template<typename T, typename C>
int _print_t(Node<T, C> *tree, int is_left, int offset, int depth, char s[20][255])
{
  char b[20];
  int width = 5;

  if (!tree) return 0;

  sprintf(b, "(%03f)", tree->key);

  int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
  int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
  for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
  for (int i = 0; i < width; i++)
    s[2 * depth][offset + left + i] = b[i];

  if (depth && is_left) {

    for (int i = 0; i < width + right; i++)
      s[2 * depth - 1][offset + left + width/2 + i] = '-';

    s[2 * depth - 1][offset + left + width/2] = '+';
    s[2 * depth - 1][offset + left + width + right + width/2] = '+';

  } else if (depth && !is_left) {

    for (int i = 0; i < left + width; i++)
      s[2 * depth - 1][offset - width/2 + i] = '-';

    s[2 * depth - 1][offset + left + width/2] = '+';
    s[2 * depth - 1][offset - width/2 - 1] = '+';
  }
#endif

  return left + width + right;
}

template <typename T, typename C>
void prettyPrint(std::ostream &os, Node<T, C> *tree)
{
  if(tree == nullptr) {
    os << "Tree-is-nullptr" << endl;
    return;
  }
  char s[20][255];
  for (int i = 0; i < 20; i++)
    sprintf(s[i], "%80s", " ");

  _print_t(tree, 0, 0, 0, s);

  for (int i = 0; i < 20; i++)
    os << string(s[i]) << endl;
}

template <typename T, typename C>
void prettyPrint(Node<T, C> *tree)
{
  prettyPrint(cout, tree);
}

#endif //GEO_PROJ_PRETTYPRINTNODE_H
