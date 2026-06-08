#include <cstddef>
#include <new>
#include <stdexcept>
#include <utility>

namespace shirokov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class BSTConstIterator;

  template< class Key, class Value, class Compare >
  class BSTIterator
  {
  public:
    explicit BSTIterator(typename BSTree< Key, Value, Compare >::Node* curr);
    explicit BSTIterator(const BSTConstIterator< Key, Value, Compare >& rhs);
    bool operator==(const BSTIterator& rhs) const noexcept;
    bool operator!=(const BSTIterator& rhs) const noexcept;
    BSTIterator& operator++();
    std::pair< Key, Value& > operator*();

  private:
    typename BSTree< Key, Value, Compare >::Node* curr_;
    friend BSTree< Key, Value, Compare >;
  };

  template< class Key, class Value, class Compare >
  class BSTConstIterator
  {
  public:
    explicit BSTConstIterator(const typename BSTree< Key, Value, Compare >::Node* curr);
    bool operator==(const BSTConstIterator& rhs) const noexcept;
    bool operator!=(const BSTConstIterator& rhs) const noexcept;
    BSTConstIterator& operator++();
    std::pair< Key, Value > operator*() const;

  private:
    const typename BSTree< Key, Value, Compare >::Node* curr_;
    friend BSTIterator< Key, Value, Compare >;
    friend BSTree< Key, Value, Compare >;
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
    bool erase(Key k); // TODO
    bool empty() const noexcept;
    Value& at(Key key);
    const Value& at(Key key) const;
    void swap(BSTree< Key, Value, Compare >& rhs);

    using const_iterator = BSTConstIterator< Key, Value, Compare >;
    using iterator = BSTIterator< Key, Value, Compare >;
    const_iterator rotateLeft(const_iterator it);  // TODO
    const_iterator rotateRight(const_iterator it); // TODO

    const_iterator rotateLargeLeft(const_iterator it);  // TODO
    const_iterator rotateLargeRight(const_iterator it); // TODO

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
    void copyTree(Node* root, Node** result, Node* parent = nullptr);
  };
}

template< class Key, class Value, class Compare >
shirokov::BSTree< Key, Value, Compare >& shirokov::BSTree< Key, Value, Compare >::operator=(const BSTree& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  Node* newRoot = nullptr;
  copyTree(rhs.root_, &newRoot);
  clear(root_);
  root_ = newRoot;
  cmp_ = rhs.cmp_;
  return *this;
}

template< class Key, class Value, class Compare >
shirokov::BSTree< Key, Value, Compare >::BSTree(const BSTree& rhs):
  cmp_(rhs.cmp_)
{
  copyTree(rhs.root_, &root_);
}

template< class Key, class Value, class Compare >
void shirokov::BSTree< Key, Value, Compare >::copyTree(Node* root, Node** result, Node* parent)
{
  if (!root)
  {
    *result = nullptr;
    return;
  }
  Node* newRoot = new Node{root->key, root->value, nullptr, nullptr, parent};
  try
  {
    copyTree(root->lt, &(newRoot->lt), newRoot);
    copyTree(root->rt, &(newRoot->rt), newRoot);
  }
  catch (const std::bad_alloc&)
  {
    clear(newRoot);
    *result = nullptr;
    throw;
  }
  *result = newRoot;
}

template< class Key, class Value, class Compare >
size_t shirokov::BSTree< Key, Value, Compare >::height(const_iterator it)
{
  if (!it.curr_)
  {
    return 0;
  }
  return std::max(height(const_iterator{it.curr_->lt}), height(const_iterator{it.curr_->rt})) + 1;
}

template< class Key, class Value, class Compare >
size_t shirokov::BSTree< Key, Value, Compare >::height()
{
  return height(const_iterator{root_});
}

template< class Key, class Value, class Compare >
shirokov::BSTree< Key, Value, Compare >& shirokov::BSTree< Key, Value, Compare >::operator=(BSTree&& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  clear(root_);
  root_ = rhs.root_;
  rhs.root_ = nullptr;
  return *this;
}

template< class Key, class Value, class Compare >
Value& shirokov::BSTree< Key, Value, Compare >::operator[](Key key)
{
  insert(key, Value());
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
  return curr->value;
}

template< class Key, class Value, class Compare >
shirokov::BSTree< Key, Value, Compare >::BSTree(shirokov::BSTree< Key, Value, Compare >&& rhs):
  root_(rhs.root_),
  cmp_(rhs.cmp_)
{
  rhs.root_ = nullptr;
}

template< class Key, class Value, class Compare >
shirokov::BSTIterator< Key, Value, Compare >& shirokov::BSTIterator< Key, Value, Compare >::operator++()
{
  typename BSTree< Key, Value, Compare >::Node* next = nullptr;
  if (curr_->rt)
  {
    next = curr_->rt;
    while (next->lt)
    {
      next = next->lt;
    }
  }
  else if (curr_->parent && curr_->parent->lt == curr_)
  {
    next = curr_->parent;
  }
  else if (curr_->parent && curr_->parent->rt == curr_)
  {
    next = curr_->parent->parent;
  }
  this->curr_ = next;
  return *this;
}

template< class Key, class Value, class Compare >
shirokov::BSTConstIterator< Key, Value, Compare >& shirokov::BSTConstIterator< Key, Value, Compare >::operator++()
{
  typename BSTree< Key, Value, Compare >::Node* next = nullptr;
  if (curr_->rt)
  {
    next = curr_->rt;
    while (next->lt)
    {
      next = next->lt;
    }
  }
  else if (curr_->parent && curr_->parent->lt == curr_)
  {
    next = curr_->parent;
  }
  else if (curr_->parent && curr_->parent->rt == curr_)
  {
    next = curr_->parent->parent;
  }
  this->curr_ = next;
  return *this;
}

template< class Key, class Value, class Compare >
std::pair< Key, Value& > shirokov::BSTIterator< Key, Value, Compare >::operator*()
{
  return {curr_->key, curr_->value};
}

template< class Key, class Value, class Compare >
std::pair< Key, Value > shirokov::BSTConstIterator< Key, Value, Compare >::operator*() const
{
  return {curr_->key, curr_->value};
}

template< class Key, class Value, class Compare >
bool shirokov::BSTConstIterator< Key, Value, Compare >::operator==(const BSTConstIterator& rhs) const noexcept
{
  return this->curr_ == rhs.curr_;
}

template< class Key, class Value, class Compare >
bool shirokov::BSTConstIterator< Key, Value, Compare >::operator!=(const BSTConstIterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Compare >
bool shirokov::BSTIterator< Key, Value, Compare >::operator==(const BSTIterator& rhs) const noexcept
{
  return this->curr_ == rhs.curr_;
}

template< class Key, class Value, class Compare >
bool shirokov::BSTIterator< Key, Value, Compare >::operator!=(const BSTIterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Compare >
shirokov::BSTConstIterator< Key, Value, Compare >::BSTConstIterator(
    const typename BSTree< Key, Value, Compare >::Node* curr):
  curr_(curr)
{}

template< class Key, class Value, class Compare >
shirokov::BSTIterator< Key, Value, Compare >::BSTIterator(typename BSTree< Key, Value, Compare >::Node* curr):
  curr_(curr)
{}

template< class Key, class Value, class Compare >
shirokov::BSTConstIterator< Key, Value, Compare > shirokov::BSTree< Key, Value, Compare >::cbegin() const
{
  typename BSTree< Key, Value, Compare >::Node* curr = root_;
  while (curr && curr->lt)
  {
    curr = curr->lt;
  }
  return const_iterator{curr};
}

template< class Key, class Value, class Compare >
shirokov::BSTConstIterator< Key, Value, Compare > shirokov::BSTree< Key, Value, Compare >::cend() const
{
  return const_iterator{nullptr};
}

template< class Key, class Value, class Compare >
shirokov::BSTConstIterator< Key, Value, Compare > shirokov::BSTree< Key, Value, Compare >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Compare >
shirokov::BSTConstIterator< Key, Value, Compare > shirokov::BSTree< Key, Value, Compare >::end() const
{
  return cend();
}

template< class Key, class Value, class Compare >
shirokov::BSTIterator< Key, Value, Compare > shirokov::BSTree< Key, Value, Compare >::begin()
{
  return iterator(cbegin());
}

template< class Key, class Value, class Compare >
shirokov::BSTIterator< Key, Value, Compare > shirokov::BSTree< Key, Value, Compare >::end()
{
  return iterator(cend());
}

template< class Key, class Value, class Compare >
shirokov::BSTIterator< Key, Value, Compare >::BSTIterator(const BSTConstIterator< Key, Value, Compare >& rhs):
  curr_(const_cast< typename BSTree< Key, Value, Compare >::Node* >(rhs.curr_))
{}

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
