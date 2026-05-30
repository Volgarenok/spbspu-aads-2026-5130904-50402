#include <cstddef>
#include <utility>

namespace shirokov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class BSTIterator
  {
  public:
    explicit BSTIterator(typename BSTree< Key, Value, Compare >::Node* curr);
    bool operator==(const BSTIterator& rhs) const noexcept;
    bool operator!=(const BSTIterator& rhs) const noexcept;
    BSTIterator& operator++();
    std::pair< Key, Value > operator*() const;

  private:
    typename BSTree< Key, Value, Compare >::Node* curr;
  };

  template< class Key, class Value, class Compare >
  class BSTConstIterator
  {
  public:
    explicit BSTConstIterator(typename BSTree< Key, Value, Compare >::Node* curr);
    bool operator==(const BSTConstIterator& rhs) const noexcept;
    bool operator!=(const BSTConstIterator& rhs) const noexcept;
    BSTConstIterator& operator++();
    std::pair< Key, Value > operator*() const;

  private:
    const typename BSTree< Key, Value, Compare >::Node* curr;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    BSTree();
    BSTree(const BSTree& rhs);
    BSTree(BSTree&& rhs);
    ~BSTree();
    BSTree& operator=(const BSTree& rhs);
    BSTree& operator=(BSTree&& rhs);
    Value& operator[](Key key);

    bool insert(Key k, Value v);
    bool contains(Key k);
    bool erase(Key k);
    void swap(BSTree& rhs);
    bool empty() const noexcept;
    Value& at(Key key);
    const Value& at(Key key) const;

    using const_iterator = BSTConstIterator< Key, Value, Compare >;
    using iterator = BSTIterator< Key, Value, Compare >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

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
  else if (parent && cmp_(curr->key, key))
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
