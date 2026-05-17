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
    explicit BSTIterator(NodeBiTree< Key, Value > * node = nullptr);
    std::pair< const Key, Value > operator*() const;

    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator & other) const;
    bool operator!=(const BSTIterator & other) const;

    template< class Key, class Value, class Compare >
    friend class BSTree;

  private:
    NodeBiTree< Key, Value > * node_;

    NodeBiTree< Key, Value > * goLeftMost(NodeBiTree< Key, Value > * p);
    NodeBiTree< Key, Value > * goRightMost(NodeBiTree< Key, Value > * p);
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    explicit BSTIterator(NodeBiTree< Key, Value > * node = nullptr);
    std::pair< const Key, Value > operator*() const;

    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator & other) const;
    bool operator!=(const BSTIterator & other) const;

    template< class Key, class Value, class Compare >
    friend class BSTree;

  private:
    NodeBiTree< Key, Value > * node_;

    NodeBiTree< Key, Value > * goLeftMost(NodeBiTree< Key, Value > * p);
    NodeBiTree< Key, Value > * goRightMost(NodeBiTree< Key, Value > * p);
  };
}

template< class Key, class Value >
afanasev::BSTIterator< Key, Value > & afanasev::BSTIterator< Key, Value >::
operator++()
{
  if (!node_)
  {
    return *this;
  }

  if (node_->right_ != nullptr && node_->right_ != node_)
  {
    node_ = goLeftMost(node_->right_);
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
  if (node_ == nullptr)
  {
    return *this;
  }

  if (node_->left_ && node_->left_ != node_)
  {
    node_ = goRightMost(node_->left_);
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
goLeftMost(NodeBiTree< Key, Value > * p)
{
  if (!p)
  {
    return nullptr;
  }

  while (p->left_ && p->left_ != p)
  {
    p = p->left_;
  }
  return p;
}

template< class Key, class Value >
afanasev::NodeBiTree< Key, Value > * afanasev::BSTIterator< Key, Value >::
goRightMost(NodeBiTree< Key, Value > * p)
{
  if (!p)
  {
    return nullptr;
  }

  while (p->right_ && p->right_ != p)
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
BSTIterator(NodeBiTree< Key, Value > * node):
  node_(node)
{}

#endif
