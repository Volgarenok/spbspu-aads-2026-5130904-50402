#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <List.hpp>
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace lavrentev
{
  template< class Key, class Value, class Compare >
  struct BSTree;
}

using BSTList = lavrentev::List<lavrentev::BSTree<size_t, std::string, std::less<size_t>>>;
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
    void drop(const Key &k);
    bool has(const Key &k) const;

    using const_iterator = BSTConstIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;

    void setName(std::string name);
    std::string getName();

  private:
    struct Node
    {
      Key key_;
      Value value_;
      Node *left_ = nullptr;
      Node *right_ = nullptr;
      size_t height_;
    };
    Node *fakeroot_;
    Compare compare_;
    std::string name_;

    void clear(Node *fakeroot);
    Node *copyNodes(Node *other);
    void swap(BSTree &other) noexcept;
    Value &insertNode(Node* &node, Key k, Value v, bool isOperator);
    Node *fallLeft(Node *node);
    void updateHeight(Node *node);
    size_t getHeight(Node *node);
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
Value &lavrentev::BSTree<Key, Value, Compare>::insertNode(Node* &node, Key k, Value v, bool isOperator)
{
  if (node == nullptr)
  {
    node = new Node();
    node->key_ = k;
    node->value_ = v;
    node->height_ = 1;
    return node->value_;
  }

  Value* result = nullptr;

  if (compare_(k, node->key_))
  {
    result = &insertNode(node->left_, k, v, isOperator);
  }
  else if (compare_(node->key_, k))
  {
    result = &insertNode(node->right_, k, v, isOperator);
  }
  else
  {
    if (isOperator)
    {
      return node->value_;
    }
    else
    {
      throw std::invalid_argument("Key already exists");
    }
  }

  updateHeight(node);
  return *result;
}

template< class Key, class Value, class Compare >
Value &lavrentev::BSTree<Key, Value, Compare>::operator[](const Key &k)
{
  Value &res = insertNode(fakeroot_->left_, k, Value(), true);
  updateHeight(fakeroot_);
  return res;
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
  insertNode(fakeroot_->left_, k, v, false);
  updateHeight(fakeroot_);
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
void lavrentev::BSTree<Key, Value, Compare>::drop(const Key &k)
{
  Node *parent = fakeroot_;
  Node *curr = fakeroot_->left_;

  while (curr)
  {
    if (compare_(k, curr->key_))
    {
      parent = curr;
      curr = curr->left_;
    }
    else if (compare_(curr->key_, k))
    {
      parent = curr;
      curr = curr->right_;
    }
    else break;
  }

  if (!curr) throw std::out_of_range("Key does not exist");

  if (curr->left_ && curr->right_)
  {
    Node *newCurr = fallLeft(curr->right_);
    Node *newCurrParent = curr;
    if (newCurrParent->right_ != newCurr)
    {
      newCurrParent = curr->right_;
      while (newCurrParent->left_ != newCurr)
      {
        newCurrParent = newCurrParent->left_;
      }
    }

    curr->key_ = newCurr->key_;
    curr->value_ = newCurr->value_;
    parent = newCurrParent;
    curr = newCurr;
  }

  Node *child;
  if (curr->left_)
  {
    child = curr->left_;
  }
  else
  {
    child = curr->right_;
  }

  if (parent->left_ == curr)
  {
    parent->left_ = child;
  }
  else
  {
    parent->right_ = child;
  }

  delete curr;
}

template< class Key, class Value, class Compare >
typename lavrentev::BSTree<Key, Value, Compare>::Node
  *lavrentev::BSTree<Key, Value, Compare>::fallLeft(Node *node)
{
  if (!node)
  {
    return node;
  }
  while(node->left_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::setName(std::string name)
{
  name_ = name;
}

template< class Key, class Value, class Compare >
std::string lavrentev::BSTree<Key, Value, Compare>::getName()
{
  return name_;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::updateHeight(Node *node)
{
  if (node)
  {
    size_t h1 = getHeight(node->left_);
    size_t h2 = getHeight(node->right_);
    if (h1 > h2)
    {
      node->height_ = 1 + h1;
    }
    else
    {
      node->height_ = 1 + h2;
    }
  }
}

template< class Key, class Value, class Compare >
size_t lavrentev::BSTree<Key, Value, Compare>::getHeight(Node *node)
{
  if (node)
  {
    return node->height_;
  }
  else
  {
    return 0;
  }
}

#endif
