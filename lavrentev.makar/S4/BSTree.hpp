#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <List.hpp>
#include <iostream>
#include <cstddef>
#include <stdexcept>

namespace lavrentev
{
  template< class Key, class Value, class Compare >
  struct BSTree;
}

using BSTList = lavrentev::List<lavrentev::BSTree<std::string, size_t, std::less<std::string>>>;
using cmd_t = void (*)(std::istream &in, BSTList);

namespace lavrentev
{
  
  template< class Key, class Value >
  class BSTIterator {

  };

  template< class Key, class Value >
  class BSTConstIterator {

  };

  template< class Key, class Value, class Compare >
  struct BSTree {

  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other) noexcept;
    BSTree &operator=(BSTree other);
    Value &operator[](const Key &k);

    void push(Key k, Value v);
    const Value &get(const Key &k) const;
    void drop(Key k);
    bool has(const Key &k) const;

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
    Value &insertNode(Key k, Value v, bool flag);
  };

  void print(std::istream &in, BSTList);
  void complement(std::istream &in, BSTList);
  void intersect(std::istream &in, BSTList);
  void unionn(std::istream &in, BSTList);
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
Value &lavrentev::BSTree<Key, Value, Compare>::insertNode(Key k, Value v, bool isOperator)
{
  Node *curr = fakeroot_;
  Node *next = fakeroot_->left_;
  while (next)
  {
    curr = next;
    if (compare_(k, curr->key_))
    {
      next = curr->left_;
    }
    else if (compare_(curr->key_, k))
    {
      next = curr->right_;
    }
    else
    {
      if (isOperator)
      {
        return curr->value_;
      }
      else
      {
        throw std::invalid_argument("Value is already exists");
      }
    }
  }
  Node *newNode = new Node;
  newNode->key_ = k;
  newNode->value_ = v;
  if (curr == fakeroot_ || compare_(k, curr->key_))
  {
    curr->left_ = newNode;
  }
  else
  {
    curr->right_ = newNode;
  }
  return newNode->value_;
}

template< class Key, class Value, class Compare >
Value &lavrentev::BSTree<Key, Value, Compare>::operator[](const Key &k)
{
  return insertNode(k, Value(), true);
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

template< class Key, class Value, class Compare >
typename lavrentev::BSTree<Key, Value, Compare>::Node
  *lavrentev::BSTree<Key, Value, Compare>::copyNodes(Node *other)
{
  if (!other)
  {
    return nullptr;
  }
  Node* newBST = new Node;
  newBST->key_ = other->key_;
  newBST->value_ = other->value_;
  newBST->left_ = copyNodes(other->left_);
  newBST->right_ = copyNodes(other->right_);
  return newBST;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::swap(BSTree &other) noexcept
{
  std::swap(fakeroot_, other.fakeroot_);
  std::swap(compare_, other.compare_);
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::push(Key k, Value v)
{
  insertNode(k, v, false);
}

template< class Key, class Value, class Compare >
const Value &lavrentev::BSTree<Key, Value, Compare>::get(const Key &k) const
{
  Node *curr = fakeroot_->left_;
  while (curr)
  {
    if (compare_(k, curr->key_))
    {
      curr = curr->left_;
    }
    else if (compare_(curr->key_, k))
    {
      curr = curr->right_;
    }
    else
    {
      return curr->value_;
    }
  }
  throw std::out_of_range("Key does not exist");
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::drop(Key k)
{
  Node *curr = fakeroot_;
  Node *next = fakeroot_->left_;
  while (next)
  {
    curr = next;
    if (compare_(k, curr->key_))
    {
      next = curr->left_;
    }
    else if (compare_(curr->key_, k))
    {
      next = curr->right_;
    }
    else
    {
      // Я ЕБУ???
    }
  }
  throw std::out_of_range("Key does not exist");
}

#endif
