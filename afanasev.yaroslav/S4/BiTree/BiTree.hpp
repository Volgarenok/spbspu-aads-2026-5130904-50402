#ifndef BITREE_HPP
#define BITREE_HPP

#include <utility>

namespace afanasev
{
  template<class Key, class Value>
  class BSTConstIterator;

  template< class Key, class Value >
  struct NodeBiTree
  {
    Key key_;
    Value val_;
    NodeBiTree * left_, * right_;
    NodeBiTree * parent_;

    NodeBiTree(const Key & key, const Value & val, NodeBiTree * parent):
      key_(key),
      val_(val),
      left_(nullptr),
      right_(nullptr),
      parent_(parent)
    {}

    NodeBiTree(Key && key, Value && val, NodeBiTree * parent):
      key_(std::move(key)),
      val_(std::move(val)),
      left_(nullptr),
      right_(nullptr),
      parent_(parent)
    {}

    NodeBiTree():
      key_(),
      val_(),
      left_(this),
      right_(this),
      parent_(nullptr)
    {}
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:

    BSTree();

    ~BSTree();
    void clear(NodeBiTree< Key, Value > * node) noexcept;

    void push(Key && k, Value && v);
    void push(const Key & k, const Value & v);
    Value get(const Key & k) const;
    Value drop(const Key & k);

    using const_iterator = BSTConstIterator< Key, Value >;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height() const;
    size_t height(const_iterator it) const;

  private:
    NodeBiTree< Key, Value > sentinel_;
    NodeBiTree< Key, Value > * root_;
    size_t size_;
    Compare comp_;

    NodeBiTree< Key, Value > * findNode(const Key & k) const;
  };
}

template< class Key, class Value, class Compare >
Value afanasev::BSTree< class Key, class Value, class Compare >::
get(const Key & k) const
{
  NodeBiTree< Key, Value > * node = findNode(k);

  if (node == &sentinel_)
  {
    throw std::out_of_range("Key not found");
  }
  return &node->val_;
}


template< class Key, class Value, class Compare >
afanasev::NodeBiTree< Key, Value > * afanasev::BSTree< class Key, class Value, class Compare >::
findNode(const Key & k) const
{
  NodeBiTree< Key, Value > * cur = root_;

  while (cur != &sentinel_)
  {
    if (comp_(k, cur->key_))
    {
      cur = cur->left_;
    }
    else if (comp_(cur->key_, k))
    {
      cur = cur->right_;
    }
    else
    {
      return cur;
    }
  }
  return &sentinel_;
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< class Key, class Value, class Compare >::
clear(NodeBiTree< Key, Value > * node) noexcept
{
  if (node == &sentinel_)
  {
    return;
  }
  clear(node->left_);
  clear(node->right_);
  delete node;
  size_--;
}

template< class Key, class Value, class Compare >
afanasev::BSTree< class Key, class Value, class Compare >::
~BSTree()
{
  clear(root_);
  root_ = &sentinel_;
}

template< class Key, class Value, class Compare >
afanasev::BSTree< class Key, class Value, class Compare >::
BSTree():
  sentinel_(),
  root_(&sentinel_)
  size_(0),
  comp_()
{}


#endif
