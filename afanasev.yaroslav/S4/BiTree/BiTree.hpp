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
    NodeBiTree<Key, Value> * fallLeft(NodeBiTree<Key, Value> * node) const;
  };
}

template< class Key, class Value, class Compare >
Value afanasev::BSTree< Key, Value, Compare >::
drop(const Key & k)
{
  NodeBiTree< Key, Value > * node = findNode(k);

  if (node == &sentinel_)
  {
    throw std::out_of_range("Key not found");
  }

  Value result = std::move(node->val_);

  if (node->left_ != &sentinel_ && node->right_ != &sentinel_)
  {
    NodeBiTree< Key, Value > * succ = fallLeft(node->right_);

    node->key_ = std::move(succ->key_);
    node->val_ = std::move(succ->val_);

    node = succ;
  }

  NodeBiTree< Key, Value > * child = (node->left_ != &sentinel_) ? node->left_ : node->right_;

  if (child != &sentinel_)
  {
    child->parent_ = node->parent_;
  }

  if (node->parent_ == &sentinel_)
  {
    root_ = child;
  }
  else if (node == node->parent_->left_)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
  }

  delete node;
  --size_;
  return result;
}

template< class Key, class Value, class Compare >
afanasev::NodeBiTree< Key, Value > * afanasev::BSTree< class Key, class Value, class Compare >::
fallLeft(NodeBiTree< Key, Value > * node) const
{
  while (node != &sentinel_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< class Key, class Value, class Compare >::
push(const Key & k, const Value & v)
{
    if (root_ == &sentinel_)
  {
    root_ = new NodeBiTree< Key, Value >(k, v, &sentinel_);
    root_->left_ = root_->right_ = &sentinel_;
    ++size_;
    return;
  }

  NodeBiTree< Key, Value > * cur = root_;

  while (true)
  {
    if (comp_(k, cur->key_))
    {
      if (cur->left_ == &sentinel_)
      {
        cur->left_ = new NodeBiTree< Key, Value >(k, v, cur);
        cur->left_->left_ = cur->left_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->left_;

    }
    else if (comp_(cur->key_, k))
    {
      if (cur->right_ == &sentinel_)
      {
        cur->right_ = new NodeBiTree< Key, Value >(k, v, cur);
        cur->right_->left_ = cur->right_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->right_;
    }
    else
    {
      cur->val_ = v;
      return;
    }
  }
}

template< class Key, class Value, class Compare >
void afanasev::BSTree< class Key, class Value, class Compare >::
push(Key && k, Value && v)
{
  if (root_ == &sentinel_)
  {
    root_ = new NodeBiTree< Key, Value >(std::move(k), std::move(v), &sentinel_);
    root_->left_ = root_->right_ = &sentinel_;
    ++size_;
    return;
  }

  NodeBiTree< Key, Value > * cur = root_;

  while (true)
  {
    if (comp_(k, cur->key_))
    {
      if (cur->left_ == &sentinel_)
      {
        cur->left_ = new NodeBiTree< Key, Value >(std::move(k), std::move(v), cur);
        cur->left_->left_ = cur->left_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->left_;

    }
    else if (comp_(cur->key_, k))
    {
      if (cur->right_ == &sentinel_)
      {
        cur->right_ = new NodeBiTree< Key, Value >(std::move(k), std::move(v), cur);
        cur->right_->left_ = cur->right_->right_ = &sentinel_;
        ++size_;
        return;
      }

      cur = cur->right_;
    }
    else
    {
      cur->val_ = std::move(v);
      return;
    }
  }
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
