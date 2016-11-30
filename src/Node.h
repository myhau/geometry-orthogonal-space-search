#ifndef GEO_PROJ_NODE_H
#define GEO_PROJ_NODE_H

template<typename T>
struct Node {
  Node *left;
  Node *right;
  double key;
  bool isLeaf;
  T value;

  Node(double key, T value) : key(key), value(value), isLeaf(true), left(nullptr), right(nullptr) {}

  Node(double key, T value, bool leaf) : key(key), value(value), isLeaf(leaf), left(nullptr),
                                         right(nullptr) {}

  Node(double key) : key(key), isLeaf(false), left(nullptr), right(nullptr) {}

};

template<typename T>
Node<T> make_node(double key, T value, bool leaf = true) {
  return Node<T>(key, value, leaf);
}

template<typename T>
Node<T> make_node(double key) {
  return Node<T>(key);
}


template<typename T>
Node<T> *make_node_ptr(double key, T value, bool leaf = true) {
  return new Node<T>(key, value, leaf);
}

template<typename T>
Node<T> *make_node_ptr(double key) {
  return new Node<T>(key);
}

//template<typename T>
//struct MedianWithAssociatedStructure {
//
//};

template<typename T>
void destruct(Node<T> *n) {
  if (n == nullptr) {
    return;
  }
  destruct(n->left);
  destruct(n->right);
  delete n;
}


#endif //GEO_PROJ_NODE_H
