#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <functional>
#include "bstree.hpp"

namespace pozdnyakov
{

  struct EmptyValue
  {
    bool operator==(const EmptyValue &) const
    {
      return true;
    }
  };

  template< class Key, class Value, class Compare = std::less< Key > >
  class Map
  {
  private:
    BSTree< Key, Value, Compare, false > tree;

  public:
    using iterator = typename BSTree< Key, Value, Compare, false >::iterator;
    using const_iterator = typename BSTree< Key, Value, Compare, false >::const_iterator;

    void insert(const Key &key, const Value &value)
    {
      tree.push(key, value);
    }
    void erase(const Key &key)
    {
      tree.remove(key);
    }
    Value &at(const Key &key)
    {
      return tree.get(key);
    }
    const Value &at(const Key &key) const
    {
      return tree.get(key);
    }
    bool contains(const Key &key) const
    {
      return tree.contains(key);
    }
    bool empty() const
    {
      return tree.empty();
    }

    iterator begin()
    {
      return tree.begin();
    }
    iterator end()
    {
      return tree.end();
    }
    const_iterator begin() const
    {
      return tree.begin();
    }
    const_iterator end() const
    {
      return tree.end();
    }
  };

  template< class Key, class Compare = std::less< Key > >
  class Set
  {
  private:
    BSTree< Key, EmptyValue, Compare, false > tree;

  public:
    using iterator = typename BSTree< Key, EmptyValue, Compare, false >::iterator;
    using const_iterator = typename BSTree< Key, EmptyValue, Compare, false >::const_iterator;

    void insert(const Key &key)
    {
      tree.push(key, EmptyValue{});
    }
    void erase(const Key &key)
    {
      tree.remove(key);
    }
    bool contains(const Key &key) const
    {
      return tree.contains(key);
    }
    bool empty() const
    {
      return tree.empty();
    }

    iterator begin()
    {
      return tree.begin();
    }
    iterator end()
    {
      return tree.end();
    }
    const_iterator begin() const
    {
      return tree.begin();
    }
    const_iterator end() const
    {
      return tree.end();
    }
  };

  template< class Key, class Value, class Compare = std::less< Key > >
  class MultiMap
  {
  private:
    BSTree< Key, Value, Compare, true > tree;

  public:
    using iterator = typename BSTree< Key, Value, Compare, true >::iterator;
    using const_iterator = typename BSTree< Key, Value, Compare, true >::const_iterator;

    void insert(const Key &key, const Value &value)
    {
      tree.push(key, value);
    }
    void erase(const Key &key)
    {
      tree.remove(key);
    }
    bool contains(const Key &key) const
    {
      return tree.contains(key);
    }
    bool empty() const
    {
      return tree.empty();
    }

    iterator begin()
    {
      return tree.begin();
    }
    iterator end()
    {
      return tree.end();
    }
    const_iterator begin() const
    {
      return tree.begin();
    }
    const_iterator end() const
    {
      return tree.end();
    }
  };

  template< class Key, class Compare = std::less< Key > >
  class MultiSet
  {
  private:
    BSTree< Key, EmptyValue, Compare, true > tree;

  public:
    using iterator = typename BSTree< Key, EmptyValue, Compare, true >::iterator;
    using const_iterator = typename BSTree< Key, EmptyValue, Compare, true >::const_iterator;

    void insert(const Key &key)
    {
      tree.push(key, EmptyValue{});
    }
    void erase(const Key &key)
    {
      tree.remove(key);
    }
    bool contains(const Key &key) const
    {
      return tree.contains(key);
    }
    bool empty() const
    {
      return tree.empty();
    }

    iterator begin()
    {
      return tree.begin();
    }
    iterator end()
    {
      return tree.end();
    }
    const_iterator begin() const
    {
      return tree.begin();
    }
    const_iterator end() const
    {
      return tree.end();
    }
  };

}

#endif
