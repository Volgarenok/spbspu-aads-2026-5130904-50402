#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP

#include <utility>
#include "treenode.hpp"

namespace karpovich
{
  template < class Key, class Value, class Compare >
  class BSTree;

  template < class Key, class Value >
  class BSTConstIterator
  {
  public:
    explicit BSTConstIterator(TreeNode< Key, Value > *node = nullptr);
    std::pair< const Key, Value > operator*() const;
    BSTConstIterator &operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator &operator--();
    BSTConstIterator operator--(int);
    bool operator==(const BSTConstIterator &other) const;
    bool operator!=(const BSTConstIterator &other) const;

    template < class K, class V, class C >
    friend class BSTree;

  private:
    TreeNode< Key, Value > *node_;
  };

  template < class Key, class Value >
  class BSTIterator
  {
  public:
    explicit BSTIterator(TreeNode< Key, Value > *node = nullptr);
    std::pair< const Key, Value > operator*() const;
    BSTIterator &operator++();
    BSTIterator operator++(int);
    BSTIterator &operator--();
    BSTIterator operator--(int);
    bool operator==(const BSTIterator &other) const;
    bool operator!=(const BSTIterator &other) const;

    template < class K, class V, class C >
    friend class BSTree;

  private:
    TreeNode< Key, Value > *node_;
  };
}

template < class Key, class Value >
karpovich::BSTConstIterator< Key, Value >::BSTConstIterator(TreeNode< Key, Value > *node):
  node_(node)
{}

template < class Key, class Value >
std::pair< const Key, Value > karpovich::BSTConstIterator< Key, Value >::operator*() const
{
  return std::pair< const Key, Value >(node_->key_, node_->value_);
}

template < class Key, class Value >
karpovich::BSTConstIterator< Key, Value > &karpovich::BSTConstIterator< Key, Value >::operator++()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->right_ != nullptr) {
    node_ = node_->right_;
    while (node_->left_ != nullptr) {
      node_ = node_->left_;
    }
  } else {
    TreeNode< Key, Value > *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->right_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template < class Key, class Value >
karpovich::BSTConstIterator< Key, Value > karpovich::BSTConstIterator< Key, Value >::operator++(int)
{
  BSTConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template < class Key, class Value >
karpovich::BSTConstIterator< Key, Value > &karpovich::BSTConstIterator< Key, Value >::operator--()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->left_ != nullptr) {
    node_ = node_->left_;
    while (node_->right_ != nullptr) {
      node_ = node_->right_;
    }
  } else {
    TreeNode< Key, Value > *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->left_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template < class Key, class Value >
karpovich::BSTConstIterator< Key, Value > karpovich::BSTConstIterator< Key, Value >::operator--(int)
{
  BSTConstIterator tmp = *this;
  --(*this);
  return tmp;
}

template < class Key, class Value >
bool karpovich::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator &other) const
{
  return node_ == other.node_;
}

template < class Key, class Value >
bool karpovich::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator &other) const
{
  return node_ != other.node_;
}

template < class Key, class Value >
karpovich::BSTIterator< Key, Value >::BSTIterator(TreeNode< Key, Value > *node):
  node_(node)
{}

template < class Key, class Value >
std::pair< const Key, Value > karpovich::BSTIterator< Key, Value >::operator*() const
{
  return std::pair< const Key, Value >(node_->key_, node_->value_);
}

template < class Key, class Value >
karpovich::BSTIterator< Key, Value > &karpovich::BSTIterator< Key, Value >::operator++()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->right_ != nullptr) {
    node_ = node_->right_;
    while (node_->left_ != nullptr) {
      node_ = node_->left_;
    }
  } else {
    TreeNode< Key, Value > *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->right_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template < class Key, class Value >
karpovich::BSTIterator< Key, Value > karpovich::BSTIterator< Key, Value >::operator++(int)
{
  BSTIterator tmp = *this;
  ++(*this);
  return tmp;
}

template < class Key, class Value >
karpovich::BSTIterator< Key, Value > &karpovich::BSTIterator< Key, Value >::operator--()
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->left_ != nullptr) {
    node_ = node_->left_;
    while (node_->right_ != nullptr) {
      node_ = node_->right_;
    }
  } else {
    TreeNode< Key, Value > *parent = node_->parent_;
    while (parent != nullptr && node_ == parent->left_) {
      node_ = parent;
      parent = node_->parent_;
    }
    node_ = parent;
  }
  return *this;
}

template < class Key, class Value >
karpovich::BSTIterator< Key, Value > karpovich::BSTIterator< Key, Value >::operator--(int)
{
  BSTIterator tmp = *this;
  --(*this);
  return tmp;
}

template < class Key, class Value >
bool karpovich::BSTIterator< Key, Value >::operator==(const BSTIterator &other) const
{
  return node_ == other.node_;
}

template < class Key, class Value >
bool karpovich::BSTIterator< Key, Value >::operator!=(const BSTIterator &other) const
{
  return node_ != other.node_;
}

#endif
