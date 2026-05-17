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
    NodeBiTree< Key, Value > *node_;
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
    NodeBiTree< Key, Value > *node_;
  };
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
