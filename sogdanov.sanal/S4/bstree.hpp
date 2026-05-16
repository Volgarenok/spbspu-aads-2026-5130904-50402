#ifndef BSTREE_HPP
#define BSTREE_HPP

#include "bstiterators.hpp"
#include <functional>
#include <stdexcept>
#include <algorithm>

namespace sogdanov
{

  template <class Key, class Value, class Compare = std::less<Key>>
  class BSTree
  {
  private:
    Node<Key, Value> *root;
    Node<Key, Value> *fake_leaf;
    Compare comp;
    size_t tree_size;

    void destroy(Node<Key, Value> *n);
    Node<Key, Value> *copyTree(const Node<Key, Value> *otherNode, Node<Key, Value> *parent, Node<Key, Value> *otherFake);
    size_t calculateHeight(Node<Key, Value> *n) const;

  public:
    BSTree();
    BSTree(const BSTree<Key, Value, Compare> &other);
    ~BSTree();
    BSTree<Key, Value, Compare> &operator=(const BSTree<Key, Value, Compare> &other);
    bool empty() const;

    BSTIterator<Key, Value> begin();
    BSTIterator<Key, Value> end();
    BSTConstIterator<Key, Value> begin() const;
    BSTConstIterator<Key, Value> end() const;

    BSTIterator<Key, Value> find(const Key &k);
    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);

    BSTConstIterator<Key, Value> rotateLeft(BSTConstIterator<Key, Value> it);
    BSTConstIterator<Key, Value> rotateRight(BSTConstIterator<Key, Value> it);
    BSTConstIterator<Key, Value> rotateLargeLeft(BSTConstIterator<Key, Value> it);
    BSTConstIterator<Key, Value> rotateLargeRight(BSTConstIterator<Key, Value> it);

    size_t height(BSTConstIterator<Key, Value> it) const;
    size_t height() const;
  };

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::destroy(Node<Key, Value> *n)
  {
    if (n != fake_leaf)
    {
      destroy(n->left);
      destroy(n->right);
      delete n;
    }
  }

  template <class Key, class Value, class Compare>
  Node<Key, Value> *BSTree<Key, Value, Compare>::copyTree(const Node<Key, Value> *otherNode, Node<Key, Value> *parent, Node<Key, Value> *otherFake)
  {
    if (otherNode == otherFake)
    {
      return fake_leaf;
    }
    Node<Key, Value> *newNode = new Node<Key, Value>(otherNode->data.first, otherNode->data.second, parent);
    newNode->left = copyTree(otherNode->left, newNode, otherFake);
    newNode->right = copyTree(otherNode->right, newNode, otherFake);
    return newNode;
  }

  template <class Key, class Value, class Compare>
  size_t BSTree<Key, Value, Compare>::calculateHeight(Node<Key, Value> *n) const
  {
    if (n == fake_leaf)
    {
      return 0;
    }
    return 1 + std::max(calculateHeight(n->left), calculateHeight(n->right));
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree() : tree_size(0)
  {
    fake_leaf = new Node<Key, Value>(Key(), Value(), nullptr);
    fake_leaf->left = fake_leaf;
    fake_leaf->right = fake_leaf;
    root = fake_leaf;
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree(const BSTree<Key, Value, Compare> &other) : tree_size(other.tree_size), comp(other.comp)
  {
    fake_leaf = new Node<Key, Value>(Key(), Value(), nullptr);
    fake_leaf->left = fake_leaf;
    fake_leaf->right = fake_leaf;
    root = copyTree(other.root, nullptr, other.fake_leaf);
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::~BSTree()
  {
    destroy(root);
    delete fake_leaf;
  }
}

#endif
