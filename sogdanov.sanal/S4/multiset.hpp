#ifndef MULTISET_HPP
#define MULTISET_HPP

#include "AVLTree.hpp"

namespace sogdanov {

  template< class Key, class Compare = std::less< Key > >
  class MultiSet {
  public:
    using iterator = BSTIterator< Key, Key >;
    using const_iterator = BSTConstIterator< Key, Key >;

    MultiSet();

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
    AVLTree< Key, Key, Compare, true > tree;
  };

  template< class Key, class Compare >
  MultiSet< Key, Compare >::MultiSet()
  {
  }

  template< class Key, class Compare >
  typename MultiSet< Key, Compare >::iterator MultiSet< Key, Compare >::insert(const Key &key)
  {
    return tree.push(key, key);
  }

  template< class Key, class Compare >
  void MultiSet< Key, Compare >::erase(const Key &key)
  {
    tree.drop(key);
  }

  template< class Key, class Compare >
  typename MultiSet< Key, Compare >::iterator MultiSet< Key, Compare >::find(const Key &key)
  {
    return tree.find(key);
  }

  template< class Key, class Compare >
  typename MultiSet< Key, Compare >::const_iterator MultiSet< Key, Compare >::find(const Key &key) const
  {
    return tree.find(key);
  }

  template< class Key, class Compare >
  bool MultiSet< Key, Compare >::empty() const
  {
    return tree.empty();
  }

  template< class Key, class Compare >
  size_t MultiSet< Key, Compare >::size() const
  {
    return tree.size();
  }

  template< class Key, class Compare >
  typename MultiSet< Key, Compare >::iterator MultiSet< Key, Compare >::begin()
  {
    return tree.begin();
  }

  template< class Key, class Compare >
  typename MultiSet< Key, Compare >::iterator MultiSet< Key, Compare >::end()
  {
    return tree.end();
  }

  template< class Key, class Compare >
  typename MultiSet< Key, Compare >::const_iterator MultiSet< Key, Compare >::begin() const
  {
    return tree.begin();
  }

  template< class Key, class Compare >
  typename MultiSet< Key, Compare >::const_iterator MultiSet< Key, Compare >::end() const
  {
    return tree.end();
  }

}

#endif
