#ifndef BITREE_HPP
#define BITREE_HPP

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>
#include "BiTreeNode.hpp"
#include "BiTreeIter.hpp"

namespace afanasev
{
  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:

    BSTree();

    BSTree(const BSTree & other);
    BSTree(BSTree && other) noexcept;

    BSTree & operator=(const BSTree & other);
    BSTree & operator=(BSTree && other) noexcept;

    void swap(BSTree & other) noexcept;

    ~BSTree();
    void clear() noexcept;
    
    void push(Key && k, Value && v);
    void push(const Key & k, const Value & v);

    Value drop(const Key & k);
    size_t size() const;

    Value & get(const Key & k);
    const Value & get(const Key & k) const;

    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    iterator begin();
    iterator end() noexcept;
    const_iterator begin() const;
    const_iterator end() const noexcept;

    size_t height() const;
    size_t height(const_iterator it) const;

    friend class BSTConstIterator< Key, Value >;
    friend class BSTIterator< Key, Value >;

  private:
    NodeBiTree< Key, Value > sentinel_;
    NodeBiTree< Key, Value > * root_;
    size_t size_;
    Compare comp_;

    NodeBiTree< Key, Value > * findNode(const Key & k);
    const NodeBiTree< Key, Value > * findNode(const Key & k) const;
    NodeBiTree< Key, Value > * fallLeft(NodeBiTree< Key, Value > * node) const;
    void clearTree(NodeBiTree< Key, Value > * node) noexcept;
    NodeBiTree< Key, Value > * clone(NodeBiTree< Key, Value > * src, NodeBiTree< Key, Value > * parent);
  };
}

template< class Key, class Value, class Compare >
typename afanasev::NodeBiTree< Key, Value > * afanasev::BSTree< Key, Value, Compare >::
clone(NodeBiTree< Key, Value > * src, NodeBiTree< Key, Value > * parent)
{
  if (src == &sentinel_)
  {
    return &sentinel_;
  }

  NodeBiTree< Key, Value > * newNode = new NodeBiTree< Key, Value >(src->key_, src->val_, parent);
  newNode->left_ = clone(src->left_, newNode);
  newNode->right_ = clone(src->right_, newNode);
  return newNode;
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< Key, Value, Compare >::
swap(BSTree & other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template< class Key, class Value, class Compare >
afanasev::BSTree< Key, Value, Compare >::
BSTree(const BSTree & other):
  sentinel_(),
  root_(&sentinel_),
  size_(0),
  comp_(other.comp_)
{
  if (other.root_ != &other.sentinel_)
  {
    root_ = clone(other.root_, &sentinel_);
    size_ = other.size_;
  }
}

template< class Key, class Value, class Compare >
afanasev::BSTree< Key, Value, Compare >::
BSTree(BSTree && other) noexcept:
  sentinel_(),
  root_(other.root_),
  size_(other.size_),
  comp_(std::move(other.comp_))
{
  other.root_ = &other.sentinel_;
  other.size_ = 0;
}

template< class Key, class Value, class Compare >
afanasev::BSTree< Key, Value, Compare > & afanasev::BSTree< Key, Value, Compare >::
operator=(const BSTree & other)
{
  if (this != &other)
  {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
afanasev::BSTree< Key, Value, Compare > & afanasev::BSTree< Key, Value, Compare >::
operator=(BSTree && other) noexcept
{
  if (this != &other)
  {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    comp_ = std::move(other.comp_);
    other.root_ = &other.sentinel_;
    other.size_ = 0;
  }
  return *this;
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< Key, Value, Compare >::
clearTree(NodeBiTree< Key, Value > * node) noexcept
{
  if (node == &sentinel_)
  {
    return;
  }
  clearTree(node->left_);
  clearTree(node->right_);
  delete node;
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::iterator
afanasev::BSTree< Key, Value, Compare >::
begin()
{
  return iterator(fallLeft(root_), &sentinel_);
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::iterator
afanasev::BSTree< Key, Value, Compare >::
end() noexcept
{
  return iterator(&sentinel_, &sentinel_);
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::const_iterator
afanasev::BSTree< Key, Value, Compare >::
begin() const
{
  return const_iterator(fallLeft(root_), &sentinel_);
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::const_iterator
afanasev::BSTree< Key, Value, Compare >::
end() const noexcept
{
  return const_iterator(&sentinel_, &sentinel_);
}

template< class Key, class Value, class Compare >
size_t afanasev::BSTree< Key, Value, Compare >::
size() const
{
  return size_;
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::const_iterator
afanasev::BSTree< Key, Value, Compare >::
rotateLeft(const_iterator it)
{
  NodeBiTree< Key, Value > * x = const_cast< NodeBiTree< Key, Value > * >(it.node_);
  if (x == &sentinel_ || x->right_ == &sentinel_)
  {
    return it;
  }

  NodeBiTree< Key, Value > * y = x->right_;
  x->right_ = y->left_;
  if (y->left_ != &sentinel_)
  {
    y->left_->parent_ = x;
  }

  y->parent_ = x->parent_;

  if (x->parent_ == &sentinel_)
  {
    root_ = y;
  }
  else if (x == x->parent_->left_)
  {
    x->parent_->left_ = y;
  }
  else
  {
    x->parent_->right_ = y;
  }

  y->left_ = x;
  x->parent_ = y;

  return const_iterator(y);
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::const_iterator
afanasev::BSTree< Key, Value, Compare >::
rotateRight(const_iterator it)
{
  NodeBiTree< Key, Value > * y = const_cast< NodeBiTree< Key, Value > * >(it.node_);
  if (y == &sentinel_ || y->left_ == &sentinel_)
  {
    return it;
  }

  NodeBiTree< Key, Value > * x = y->left_;
  y->left_ = x->right_;
  if (x->right_ != &sentinel_)
  {
    x->right_->parent_ = y;
  }

  x->parent_ = y->parent_;
  if (y->parent_ == &sentinel_)
  {
    root_ = x;
  }
  else if (y == y->parent_->left_)
  {
    y->parent_->left_ = x;
  }
  else
  {
    y->parent_->right_ = x;
  }

  x->right_ = y;
  y->parent_ = x;

  return const_iterator(x);
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::const_iterator
afanasev::BSTree< Key, Value, Compare >::
rotateLargeLeft(const_iterator it)
{
  NodeBiTree< Key, Value > * node = const_cast< NodeBiTree< Key, Value > * >(it.node_);
  if (node == &sentinel_ || node->left_ == &sentinel_ || node->left_->right_ == &sentinel_)
  {
    return it;
  }

  rotateRight(const_iterator(node->left_));
  return rotateLeft(it);
}

template< class Key, class Value, class Compare >
typename afanasev::BSTree< Key, Value, Compare >::const_iterator
afanasev::BSTree< Key, Value, Compare >::
rotateLargeRight(const_iterator it)
{
  NodeBiTree< Key, Value > * node = const_cast< NodeBiTree< Key, Value > * >(it.node_);
  if (node == &sentinel_ || node->right_ == &sentinel_ || node->right_->left_ == &sentinel_)
  {
    return it;
  }

  rotateLeft(const_iterator(node->right_));
  return rotateRight(it);
}

template< class Key, class Value, class Compare >
size_t afanasev::BSTree< Key, Value, Compare >::
height(const_iterator it) const
{
  NodeBiTree< Key, Value > * node = const_cast< NodeBiTree< Key, Value > * >(it.node_);

  if (node == &sentinel_)
  {
    return 0;
  }

  size_t leftH = height(const_iterator(node->left_));
  size_t rightH = height(const_iterator(node->right_));

  return 1 + std::max(leftH, rightH);
}

template< class Key, class Value, class Compare >
size_t afanasev::BSTree< Key, Value, Compare >::
height() const
{
  return height(const_iterator(root_));
}

template< class Key, class Value, class Compare >
Value afanasev::BSTree< Key, Value, Compare >::
drop(const Key & k)
{
  NodeBiTree< Key, Value > * node = findNode(k);

  if (node == &sentinel_)
  {
    throw std::out_of_range("Key not found");
  }

  Value result = std::move(node->val_);

  if (node->left_ != &sentinel_ && node->right_ != &sentinel_)
  {
    NodeBiTree< Key, Value > * succ = fallLeft(node->right_);

    node->key_ = std::move(succ->key_);
    node->val_ = std::move(succ->val_);

    node = succ;
  }

  NodeBiTree< Key, Value > * child = (node->left_ != &sentinel_) ? node->left_ : node->right_;

  if (child != &sentinel_)
  {
    child->parent_ = node->parent_;
  }

  if (node->parent_ == &sentinel_)
  {
    root_ = child;
  }
  else if (node == node->parent_->left_)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
  }

  delete node;
  --size_;
  return result;
}

template< class Key, class Value, class Compare >
afanasev::NodeBiTree< Key, Value > * afanasev::BSTree< Key, Value, Compare >::
fallLeft(NodeBiTree< Key, Value > * node) const
{
  while (node->left_ != &sentinel_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< Key, Value, Compare >::
push(const Key & k, const Value & v)
{
    if (root_ == &sentinel_)
  {
    root_ = new NodeBiTree< Key, Value >(k, v, &sentinel_);
    root_->left_ = root_->right_ = &sentinel_;
    ++size_;
    return;
  }

  NodeBiTree< Key, Value > * cur = root_;

  while (true)
  {
    if (comp_(k, cur->key_))
    {
      if (cur->left_ == &sentinel_)
      {
        cur->left_ = new NodeBiTree< Key, Value >(k, v, cur);
        cur->left_->left_ = cur->left_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->left_;

    }
    else if (comp_(cur->key_, k))
    {
      if (cur->right_ == &sentinel_)
      {
        cur->right_ = new NodeBiTree< Key, Value >(k, v, cur);
        cur->right_->left_ = cur->right_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->right_;
    }
    else
    {
      cur->val_ = v;
      return;
    }
  }
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< Key, Value, Compare >::
push(Key && k, Value && v)
{
  if (root_ == &sentinel_)
  {
    root_ = new NodeBiTree< Key, Value >(std::move(k), std::move(v), &sentinel_);
    root_->left_ = root_->right_ = &sentinel_;
    ++size_;
    return;
  }

  NodeBiTree< Key, Value > * cur = root_;

  while (true)
  {
    if (comp_(k, cur->key_))
    {
      if (cur->left_ == &sentinel_)
      {
        cur->left_ = new NodeBiTree< Key, Value >(std::move(k), std::move(v), cur);
        cur->left_->left_ = cur->left_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->left_;

    }
    else if (comp_(cur->key_, k))
    {
      if (cur->right_ == &sentinel_)
      {
        cur->right_ = new NodeBiTree< Key, Value >(std::move(k), std::move(v), cur);
        cur->right_->left_ = cur->right_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->right_;
    }
    else
    {
      cur->val_ = std::move(v);
      return;
    }
  }
}

template< class Key, class Value, class Compare >
Value & afanasev::BSTree< Key, Value, Compare >::
get(const Key & k)
{
  NodeBiTree< Key, Value > * node = findNode(k);

  if (node == &sentinel_)
  {
    throw std::out_of_range("Key not found");
  }
  return node->val_;
}

template< class Key, class Value, class Compare >
const Value & afanasev::BSTree< Key, Value, Compare >::
get(const Key & k) const
{
  const NodeBiTree< Key, Value > * node = findNode(k);

  if (node == &sentinel_)
  {
    throw std::out_of_range("Key not found");
  }
  return node->val_;
}

template< class Key, class Value, class Compare >
afanasev::NodeBiTree< Key, Value > * afanasev::BSTree< Key, Value, Compare >::
findNode(const Key & k)
{
  NodeBiTree< Key, Value > * cur = root_;

  while (cur != &sentinel_)
  {
    if (comp_(k, cur->key_))
    {
      cur = cur->left_;
    }
    else if (comp_(cur->key_, k))
    {
      cur = cur->right_;
    }
    else
    {
      return cur;
    }
  }
  return &sentinel_;
}

template< class Key, class Value, class Compare >
const afanasev::NodeBiTree< Key, Value > * afanasev::BSTree< Key, Value, Compare >::
findNode(const Key & k) const
{
  const NodeBiTree< Key, Value > * cur = root_;

  while (cur != &sentinel_)
  {
    if (comp_(k, cur->key_))
    {
      cur = cur->left_;
    }
    else if (comp_(cur->key_, k))
    {
      cur = cur->right_;
    }
    else
    {
      return cur;
    }
  }
  return &sentinel_;
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< Key, Value, Compare >::
clear() noexcept
{
  if (root_ == &sentinel_)
  {
    return;
  }
  clearTree(root_);
  root_ = &sentinel_;
  size_ = 0;
}

template< class Key, class Value, class Compare >
afanasev::BSTree< Key, Value, Compare >::
~BSTree()
{
  clear();
}

template< class Key, class Value, class Compare >
afanasev::BSTree< Key, Value, Compare >::
BSTree():
  sentinel_(),
  root_(&sentinel_),
  size_(0),
  comp_()
{}

#endif
