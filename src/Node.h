#ifndef GEO_PROJ_NODE_H
#define GEO_PROJ_NODE_H

template<typename T, typename C>
struct Node {
  Node *left;
  Node *right;
  C key;
  bool isLeaf;
  T value;

  Node(C key, T value) : key(key), value(value), isLeaf(true), left(nullptr), right(nullptr) {}

  Node(C key, T value, bool leaf) : key(key), value(value), isLeaf(leaf), left(nullptr),
                                         right(nullptr) {}

  Node(C key) : key(key), isLeaf(false), left(nullptr), right(nullptr) {}

};

template<typename T, typename Comparable>
Node<T, Comparable> make_node(Comparable key, T value, bool leaf = true) {
  return Node<T, Comparable>(key, value, leaf);
}

template<typename T, typename Comparable>
Node<T, Comparable> make_node(Comparable key) {
  return Node<T, Comparable>(key);
}


template<typename T, typename C>
Node<T, C> *make_node_ptr(C key, T value, bool leaf = true) {
  return new Node<T, C>(key, value, leaf);
}

template<typename T, typename C>
Node<T, C> *make_node_ptr(C key) {
  return new Node<T, C>(key);
}

//template<typename T>
//struct MedianWithAssociatedStructure {
//
//};

template<typename T, typename C>
void destruct(Node<T, C> *n) {
  if (n == nullptr) {
    return;
  }
  destruct(n->left);
  destruct(n->right);
  delete n;
}


#endif //GEO_PROJ_NODE_H
