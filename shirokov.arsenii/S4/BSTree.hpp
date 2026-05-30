#include <cstddef>
#include <stdexcept>
#include <utility>

namespace shirokov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class BSTIterator
  {
  public:
    explicit BSTIterator(typename BSTree< Key, Value, Compare >::Node* curr); // TODO
    bool operator==(const BSTIterator& rhs) const noexcept;                   // TODO
    bool operator!=(const BSTIterator& rhs) const noexcept;                   // TODO
    BSTIterator& operator++();                                                // TODO
    std::pair< Key, Value& > operator*() const;                               // TODO

  private:
    typename BSTree< Key, Value, Compare >::Node* curr;
  };

  template< class Key, class Value, class Compare >
  class BSTConstIterator
  {
  public:
    explicit BSTConstIterator(typename BSTree< Key, Value, Compare >::Node* curr); // TODO
    bool operator==(const BSTConstIterator& rhs) const noexcept;                   // TODO
    bool operator!=(const BSTConstIterator& rhs) const noexcept;                   // TODO
    BSTConstIterator& operator++();                                                // TODO
    std::pair< Key, Value > operator*() const;                                     // TODO

  private:
    const typename BSTree< Key, Value, Compare >::Node* curr;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    BSTree();
    BSTree(const BSTree& rhs); // TODO
    BSTree(BSTree&& rhs);      // TODO
    ~BSTree();
    BSTree& operator=(const BSTree& rhs); // TODO
    BSTree& operator=(BSTree&& rhs);      // TODO
    Value& operator[](Key key);           // TODO

    bool insert(Key k, Value v);
    bool contains(Key k);
    bool erase(Key k); // TODO
    bool empty() const noexcept;
    Value& at(Key key);
    const Value& at(Key key) const;

    using const_iterator = BSTConstIterator< Key, Value, Compare >;
    using iterator = BSTIterator< Key, Value, Compare >;
    const_iterator rotateLeft(const_iterator it);  // TODO
    const_iterator rotateRight(const_iterator it); // TODO

    const_iterator rotateLargeLeft(const_iterator it);  // TODO
    const_iterator rotateLargeRight(const_iterator it); // TODO

    size_t height(const_iterator it); // TODO
    size_t height();                  // TODO

    iterator begin();              // TODO
    const_iterator begin() const;  // TODO
    const_iterator cbegin() const; // TODO

    iterator end();              // TODO
    const_iterator end() const;  // TODO
    const_iterator cend() const; // TODO

  private:
    friend iterator;
    friend const_iterator;
    struct Node
    {
      Key key;
      Value value;
      Node *lt, *rt, *parent;
    };
    Node* root_;
    Compare cmp_;

    void clear(Node* curr);
  };
}

template< class Key, class Value, class Compare >
bool shirokov::BSTree< Key, Value, Compare >::contains(Key key)
{
  Node* curr = root_;
  while (curr)
  {
    if (cmp_(key, curr->key))
    {
      curr = curr->lt;
    }
    else if (cmp_(curr->key, key))
    {
      curr = curr->rt;
    }
    else
    {
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Compare >
Value& shirokov::BSTree< Key, Value, Compare >::at(Key key)
{
  const BSTree< Key, Value, Compare >* cthis = this;
  const Value& ret = cthis->at(key);
  return const_cast< Value& >(ret);
}

template< class Key, class Value, class Compare >
const Value& shirokov::BSTree< Key, Value, Compare >::at(Key key) const
{
  Node* curr = root_;
  while (curr)
  {
    if (cmp_(key, curr->key))
    {
      curr = curr->lt;
    }
    else if (cmp_(curr->key, key))
    {
      curr = curr->rt;
    }
    else
    {
      return curr->value;
    }
  }
  throw std::out_of_range("Undefined key");
}

template< class Key, class Value, class Compare >
bool shirokov::BSTree< Key, Value, Compare >::insert(Key key, Value value)
{
  Node* curr = root_;
  Node* parent = nullptr;
  while (curr)
  {
    if (cmp_(key, curr->key))
    {
      parent = curr;
      curr = curr->lt;
    }
    else if (cmp_(curr->key, key))
    {
      parent = curr;
      curr = curr->rt;
    }
    else
    {
      return false;
    }
  }
  curr = new Node{key, value, nullptr, nullptr, parent};
  if (!root_)
  {
    root_ = curr;
    return true;
  }
  if (parent && cmp_(curr->key, parent->key))
  {
    parent->lt = curr;
  }
  else if (parent && cmp_(parent->key, curr->key))
  {
    parent->rt = curr;
  }
  return true;
}

template< class Key, class Value, class Compare >
shirokov::BSTree< Key, Value, Compare >::BSTree():
  root_(nullptr),
  cmp_(Compare())
{}

template< class Key, class Value, class Compare >
shirokov::BSTree< Key, Value, Compare >::BSTree::~BSTree()
{
  clear(root_);
}

template< class Key, class Value, class Compare >
bool shirokov::BSTree< Key, Value, Compare >::empty() const noexcept
{
  return !root_;
}

template< class Key, class Value, class Compare >
void shirokov::BSTree< Key, Value, Compare >::clear(Node* curr)
{
  if (!curr)
  {
    return;
  }
  clear(curr->lt);
  clear(curr->rt);
  delete curr;
}
