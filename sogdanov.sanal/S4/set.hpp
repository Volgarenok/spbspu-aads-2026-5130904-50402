#ifndef SET_HPP
#define SET_HPP

#include "AVLTree.hpp"

namespace sogdanov {

  template< class Key, class Compare = std::less< Key > >
  class Set {
  public:
    using iterator = BSTIterator< Key, Key >;
    using const_iterator = BSTConstIterator< Key, Key >;

    Set();

    iterator insert(const Key &key);
    void erase(const Key &key);
    iterator find(const Key &key);
    const_iterator find(const Key &key) const;
    bool empty() const;
    size_t size() const;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

  private:
    AVLTree< Key, Key, Compare, false > tree;
  };

  template< class Key, class Compare >
  Set< Key, Compare >::Set()
  {
  }

  template< class Key, class Compare >
  typename Set< Key, Compare >::iterator Set< Key, Compare >::insert(const Key &key)
  {
    return tree.push(key, key);
  }

  template< class Key, class Compare >
  void Set< Key, Compare >::erase(const Key &key)
  {
    tree.drop(key);
  }

  template< class Key, class Compare >
  typename Set< Key, Compare >::iterator Set< Key, Compare >::find(const Key &key)
  {
    return tree.find(key);
  }

  template< class Key, class Compare >
  typename Set< Key, Compare >::const_iterator Set< Key, Compare >::find(const Key &key) const
  {
    return tree.find(key);
  }

  template< class Key, class Compare >
  bool Set< Key, Compare >::empty() const
  {
    return tree.empty();
  }

  template< class Key, class Compare >
  size_t Set< Key, Compare >::size() const
  {
    return tree.size();
  }

  template< class Key, class Compare >
  typename Set< Key, Compare >::iterator Set< Key, Compare >::begin()
  {
    return tree.begin();
  }

  template< class Key, class Compare >
  typename Set< Key, Compare >::iterator Set< Key, Compare >::end()
  {
    return tree.end();
  }

  template< class Key, class Compare >
  typename Set< Key, Compare >::const_iterator Set< Key, Compare >::begin() const
  {
    return tree.begin();
  }

  template< class Key, class Compare >
  typename Set< Key, Compare >::const_iterator Set< Key, Compare >::end() const
  {
    return tree.end();
  }

}

#endif
