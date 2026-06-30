#ifndef MAP_HPP
#define MAP_HPP

#include "AVLTree.hpp"

namespace sogdanov {

  template< class Key, class Value, class Compare = std::less< Key > >
  class Map {
  public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    Map();

    Value &operator[](const Key &key);
    Value &at(const Key &key);
    const Value &at(const Key &key) const;
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
    AVLTree< Key, Value, Compare, false > tree;
  };

  template< class Key, class Value, class Compare >
  Map< Key, Value, Compare >::Map()
  {
  }

  template< class Key, class Value, class Compare >
  Value &Map< Key, Value, Compare >::operator[](const Key &key)
  {
    iterator it = tree.find(key);
    if (it == tree.end()) {
      it = tree.push(key, Value());
    }
    return (*it).second;
  }

  template< class Key, class Value, class Compare >
  Value &Map< Key, Value, Compare >::at(const Key &key)
  {
    return tree.get(key);
  }

  template< class Key, class Value, class Compare >
  const Value &Map< Key, Value, Compare >::at(const Key &key) const
  {
    return tree.get(key);
  }

  template< class Key, class Value, class Compare >
  typename Map< Key, Value, Compare >::iterator Map< Key, Value, Compare >::insert(const Key &key, const Value &value)
  {
    return tree.push(key, value);
  }

  template< class Key, class Value, class Compare >
  void Map< Key, Value, Compare >::erase(const Key &key)
  {
    tree.drop(key);
  }

  template< class Key, class Value, class Compare >
  typename Map< Key, Value, Compare >::iterator Map< Key, Value, Compare >::find(const Key &key)
  {
    return tree.find(key);
  }

  template< class Key, class Value, class Compare >
  typename Map< Key, Value, Compare >::const_iterator Map< Key, Value, Compare >::find(const Key &key) const
  {
    return tree.find(key);
  }

  template< class Key, class Value, class Compare >
  bool Map< Key, Value, Compare >::empty() const
  {
    return tree.empty();
  }

  template< class Key, class Value, class Compare >
  size_t Map< Key, Value, Compare >::size() const
  {
    return tree.size();
  }

  template< class Key, class Value, class Compare >
  typename Map< Key, Value, Compare >::iterator Map< Key, Value, Compare >::begin()
  {
    return tree.begin();
  }

  template< class Key, class Value, class Compare >
  typename Map< Key, Value, Compare >::iterator Map< Key, Value, Compare >::end()
  {
    return tree.end();
  }

  template< class Key, class Value, class Compare >
  typename Map< Key, Value, Compare >::const_iterator Map< Key, Value, Compare >::begin() const
  {
    return tree.begin();
  }

  template< class Key, class Value, class Compare >
  typename Map< Key, Value, Compare >::const_iterator Map< Key, Value, Compare >::end() const
  {
    return tree.end();
  }

}

#endif
