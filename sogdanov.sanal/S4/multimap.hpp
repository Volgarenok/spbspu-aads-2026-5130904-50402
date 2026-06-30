#ifndef MULTIMAP_HPP
#define MULTIMAP_HPP

#include "AVLTree.hpp"

namespace sogdanov {

  template< class Key, class Value, class Compare = std::less< Key > >
  class MultiMap {
  public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    MultiMap();

    iterator insert(const Key &key, const Value &value);
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
    AVLTree< Key, Value, Compare, true > tree;
  };

  template< class Key, class Value, class Compare >
  MultiMap< Key, Value, Compare >::MultiMap()
  {
  }

  template< class Key, class Value, class Compare >
  typename MultiMap< Key, Value, Compare >::iterator MultiMap< Key, Value, Compare >::insert(const Key &key, const Value &value)
  {
    return tree.push(key, value);
  }

  template< class Key, class Value, class Compare >
  void MultiMap< Key, Value, Compare >::erase(const Key &key)
  {
    tree.drop(key);
  }

  template< class Key, class Value, class Compare >
  typename MultiMap< Key, Value, Compare >::iterator MultiMap< Key, Value, Compare >::find(const Key &key)
  {
    return tree.find(key);
  }

  template< class Key, class Value, class Compare >
  typename MultiMap< Key, Value, Compare >::const_iterator MultiMap< Key, Value, Compare >::find(const Key &key) const
  {
    return tree.find(key);
  }

  template< class Key, class Value, class Compare >
  bool MultiMap< Key, Value, Compare >::empty() const
  {
    return tree.empty();
  }

  template< class Key, class Value, class Compare >
  size_t MultiMap< Key, Value, Compare >::size() const
  {
    return tree.size();
  }

  template< class Key, class Value, class Compare >
  typename MultiMap< Key, Value, Compare >::iterator MultiMap< Key, Value, Compare >::begin()
  {
    return tree.begin();
  }

  template< class Key, class Value, class Compare >
  typename MultiMap< Key, Value, Compare >::iterator MultiMap< Key, Value, Compare >::end()
  {
    return tree.end();
  }

  template< class Key, class Value, class Compare >
  typename MultiMap< Key, Value, Compare >::const_iterator MultiMap< Key, Value, Compare >::begin() const
  {
    return tree.begin();
  }

  template< class Key, class Value, class Compare >
  typename MultiMap< Key, Value, Compare >::const_iterator MultiMap< Key, Value, Compare >::end() const
  {
    return tree.end();
  }

}

#endif
