#ifndef BITREE_ITER_HPP
#define BITREE_ITER_HPP

#include <utility>
#include "BiTree.hpp"

namespace afanasev
{
  template < class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    explicit BSTIterator(NodeBiTree< Key, Value > * node = nullptr,
      const NodeBiTree< Key, Value > * sentinel = nullptr);
    std::pair< const Key, Value > operator*() const;

    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator & other) const;
    bool operator!=(const BSTIterator & other) const;

    template< class K, class V, class C >
    friend class BSTree;

  private:
    NodeBiTree< Key, Value > * node_;
    const NodeBiTree< Key, Value > * sentinel_;

    NodeBiTree< Key, Value > * goLeftMost(NodeBiTree< Key, Value > * p,
      const NodeBiTree< Key, Value > * sentinel);
    NodeBiTree< Key, Value > * goRightMost(NodeBiTree< Key, Value > * p,
      const NodeBiTree< Key, Value > * sentinel);
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    explicit BSTConstIterator(const NodeBiTree< Key, Value > * node = nullptr,
      const NodeBiTree< Key, Value > * sentinel = nullptr);
    std::pair< const Key, Value > operator*() const;

    BSTConstIterator & operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator & operator--();
    BSTConstIterator operator--(int);

    bool operator==(const BSTConstIterator & other) const;
    bool operator!=(const BSTConstIterator & other) const;

    template< class K, class V, class C >
    friend class BSTree;

  private:
    const NodeBiTree< Key, Value > * node_;
    const NodeBiTree< Key, Value > * sentinel_;

    const NodeBiTree< Key, Value > * goLeftMost(const NodeBiTree< Key, Value > * p,
      const NodeBiTree< Key, Value > * sentinel);
    const NodeBiTree< Key, Value > * goRightMost(const NodeBiTree< Key, Value > * p,
      const NodeBiTree< Key, Value > * sentinel);
  };
}

template< class Key, class Value >
bool afanasev::BSTIterator< Key, Value >::
operator==(const BSTIterator & other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool afanasev::BSTIterator< Key, Value >::
operator!=(const BSTIterator & other) const
{
  return !(*this == other);
}

template< class Key, class Value >
afanasev::BSTIterator< Key, Value > & afanasev::BSTIterator< Key, Value >::
operator++()
{
  if (node_ == sentinel_)
  {
    return *this;
  }

  if (node_->right_ != sentinel_)
  {
    node_ = goLeftMost(node_->right_, sentinel_);
  }
  else
  {
    NodeBiTree< Key, Value > * parent = node_->parent_;
    while (parent != nullptr && parent->right_ == node_)
    {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
afanasev::BSTIterator< Key, Value > afanasev::BSTIterator< Key, Value >::
operator++(int)
{
  BSTIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value >
afanasev::BSTIterator< Key, Value > & afanasev::BSTIterator< Key, Value >::
operator--()
{
  if (node_ == sentinel_)
  {
    return *this;
  }

  if (node_->left_ != sentinel_)
  {
    node_ = goRightMost(node_->left_, sentinel_);
  }
  else
  {
    NodeBiTree< Key, Value > * parent = node_->parent_;
    while (parent != nullptr && parent->left_ == node_)
    {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
afanasev::BSTIterator< Key, Value > afanasev::BSTIterator< Key, Value >::
operator--(int)
{
  BSTIterator tmp = *this;
  --(*this);
  return tmp;
}

template< class Key, class Value >
afanasev::NodeBiTree< Key, Value > * afanasev::BSTIterator< Key, Value >::
goLeftMost(NodeBiTree< Key, Value > * p, const NodeBiTree< Key, Value > * sentinel)
{
  if (!p)
  {
    return nullptr;
  }

  while (p->left_ != sentinel)
  {
    p = p->left_;
  }
  return p;
}

template< class Key, class Value >
afanasev::NodeBiTree< Key, Value > * afanasev::BSTIterator< Key, Value >::
goRightMost(NodeBiTree< Key, Value > * p, const NodeBiTree< Key, Value > * sentinel)
{
  if (!p)
  {
    return nullptr;
  }

  while (p->right_ != sentinel)
  {
    p = p->right_;
  }
  return p;
}

template< class Key, class Value >
std::pair< const Key, Value > afanasev::BSTIterator< Key, Value >::
operator*() const
{
  return {node_->key_, node_->val_};
}

template< class Key, class Value >
afanasev::BSTIterator< Key, Value >::
BSTIterator(NodeBiTree< Key, Value > * node, const NodeBiTree< Key, Value > * sentinel):
  node_(node),
  sentinel_(sentinel)
{}


template< class Key, class Value >
afanasev::BSTConstIterator< Key, Value >::
BSTConstIterator(const NodeBiTree< Key, Value > * node, const NodeBiTree< Key, Value > * sentinel):
  node_(node),
  sentinel_(sentinel)
{}

template< class Key, class Value >
std::pair< const Key, Value > afanasev::BSTConstIterator< Key, Value >::
operator*() const
{
  return {node_->key_, node_->val_};
}

template< class Key, class Value >
afanasev::BSTConstIterator< Key, Value > & afanasev::BSTConstIterator< Key, Value >::
operator++()
{
  if (node_ == sentinel_)
  {
    return *this;
  }

  if (node_->right_ != sentinel_)
  {
    node_ = goLeftMost(node_->right_, sentinel_);
  }
  else
  {
    const NodeBiTree< Key, Value > * parent = node_->parent_;
    while (parent && parent->right_ == node_)
    {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
afanasev::BSTConstIterator< Key, Value > afanasev::BSTConstIterator< Key, Value >::
operator++(int)
{
  BSTConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value >
afanasev::BSTConstIterator< Key, Value > & afanasev::BSTConstIterator< Key, Value >::
operator--()
{
  if (node_ == sentinel_)
  {
    return *this;
  }

  if (node_->left_ != sentinel_)
  {
    node_ = goRightMost(node_->left_, sentinel_);
  }
  else
  {
    const NodeBiTree< Key, Value > * parent = node_->parent_;
    while (parent && parent->left_ == node_)
    {
      node_ = parent;
      parent = parent->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
afanasev::BSTConstIterator< Key, Value > afanasev::BSTConstIterator< Key, Value >::
operator--(int)
{
  BSTConstIterator tmp = *this;
  --(*this);
  return tmp;
}

template< class Key, class Value >
bool afanasev::BSTConstIterator< Key, Value >::
operator==(const BSTConstIterator & other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool afanasev::BSTConstIterator< Key, Value >::
operator!=(const BSTConstIterator & other) const
{
  return !(*this == other);
}

template< class Key, class Value >
const afanasev::NodeBiTree< Key, Value > * afanasev::BSTConstIterator< Key, Value >::
goLeftMost(const NodeBiTree< Key, Value > * p, const NodeBiTree< Key, Value > * sentinel)
{
  if (!p)
  {
    return nullptr;
  }

  while (p->left_ != sentinel)
  {
    p = p->left_;
  }
  return p;
}

template< class Key, class Value >
const afanasev::NodeBiTree< Key, Value > * afanasev::BSTConstIterator< Key, Value >::
goRightMost(const NodeBiTree< Key, Value > * p, const NodeBiTree< Key, Value > * sentinel)
{
  if (!p)
  {
    return nullptr;
  }

  while (p->right_ != sentinel)
  {
    p = p->right_;
  }
  return p;
}

#endif
