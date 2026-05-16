#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <iostream>
#include <cstddef>

namespace lavrentev
{
  template< class Key, class Value >
  class BSTIterator {

  };

  template< class Key, class Value >
  class BSTConstIterator {

  };

  template< class Key, class Value, class Compare >
  class BSTree {

  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other) noexcept;
    BSTree &operator=(BSTree other);

    void push(Key k, Value v);
    Value get(Key k) const;
    void drop(Key k);

    using const_iterator = BSTConstIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;

  private:
    struct Node
    {
      Key key_;
      Value value_;
      Node *left_ = nullptr;
      Node *right_ = nullptr;
    };
    Node *fakeroot_;
    Compare compare_;

    void clear(Node *fakeroot);
    Node *copyNodes(Node *other);
    void swap(BSTree &other) noexcept;
  };
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::BSTree():
  fakeroot_(new Node{Key(), Value()}),
  compare_(Compare())
{}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::~BSTree()
{
  clear(fakeroot_);
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::BSTree(const BSTree &other)
{
  fakeroot_ = new Node();
  fakeroot_->left_ = copyNodes(other.fakeroot_->left_);
  compare_ = other.compare_;
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::BSTree(BSTree &&other) noexcept:
  BSTree()
{
  swap(other);
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare> &lavrentev::BSTree<Key, Value, Compare>::operator=(BSTree other)
{
  swap(other);
  return *this;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::swap(BSTree &other) noexcept
{
  std::swap(fakeroot_, other.fakeroot_);
  std::swap(compare_, other.compare_);
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::clear(Node *root)
{
  if (!root)
  {
    return;
  }
  clear(root->left_);
  clear(root->right_);
  delete root;
}

#endif
