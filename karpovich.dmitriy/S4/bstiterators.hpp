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
    const std::pair< const Key, Value > &operator*() const;
    BSTConstIterator &operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator &operator--();
    BSTConstIterator operator--(int);
    bool operator==(const BSTConstIterator &other) const;
    bool operator!=(const BSTConstIterator &other) const;
    friend class BSTree< Key, Value, class Compare >;

  private:
    TreeNode< Key, Value > *node_;
  };

  template < class Key, class Value >
  class BSTIterator
  {
  public:
    std::pair< const Key, Value > &operator*() const;
    BSTIterator &operator++();
    BSTIterator operator++(int);
    BSTIterator &operator--();
    BSTIterator operator--(int);
    bool operator==(const BSTIterator &other) const;
    bool operator!=(const BSTIterator &other) const;
    friend class BSTree< Key, Value, class Compare >;

  private:
    TreeNode< Key, Value > *node_;
  };
}

#endif
