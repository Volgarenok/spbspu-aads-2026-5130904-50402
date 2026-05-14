#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include "bstiterators.hpp"
#include "treenode.hpp"

namespace karpovich
{
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other) noexcept;
    ~BSTree();

    BSTree &operator=(const BSTree &other);
    BSTree &operator=(BSTree &&other) noexcept;

    bool empty() const;
    size_t size() const;

    const Value &at(const Key &k) const;
    Value &at(const Key &k);

    void push(const Key &k, const Value &v);
    void push(Key &&k, Value &&v);
    Value drop(const Key &k);

    void clear() noexcept;
    void swap(BSTree &other) noexcept;

    iterator begin();
    iterator end() noexcept;
    const_iterator begin() const;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height() const;
    size_t height(const_iterator it) const;

    friend class BSTConstIterator< Key, Value >;
    friend class BSTIterator< Key, Value >;

  private:
    TreeNode< Key, Value > *root_;
    size_t size_;
    Compare comp_;

    TreeNode< Key, Value > *clone(TreeNode< Key, Value > *src, TreeNode< Key, Value > *parent);
    TreeNode< Key, Value > *findNode(const Key &k) const;
    TreeNode< Key, Value > *fallLeft(TreeNode< Key, Value > *node) const;
    void clearImpl(TreeNode< Key, Value > *node) noexcept;
    size_t calcHeight(TreeNode< Key, Value > *node) const;
  };
}

template < class Key, class Value, class Compare >
karpovich::BSTree< Key, Value, Compare >::BSTree():
  root_(nullptr),
  size_(0),
  comp_()
{}

template < class Key, class Value, class Compare >
karpovich::BSTree< Key, Value, Compare >::BSTree(const BSTree &other):
  root_(nullptr),
  size_(0),
  comp_(other.comp_)
{
  root_ = clone(other.root_, nullptr);
  size_ = other.size_;
}

template < class Key, class Value, class Compare >
karpovich::BSTree< Key, Value, Compare >::BSTree(BSTree &&other) noexcept:
  root_(other.root_),
  size_(other.size_),
  comp_(std::move(other.comp_))
{
  other.root_ = nullptr;
  other.size_ = 0;
}

template < class Key, class Value, class Compare >
karpovich::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}

template < class Key, class Value, class Compare >
karpovich::BSTree< Key, Value, Compare > &karpovich::BSTree< Key, Value, Compare >::operator=(const BSTree &other)
{
  if (this != std::addressof(other)) {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template < class Key, class Value, class Compare >
karpovich::BSTree< Key, Value, Compare > &karpovich::BSTree< Key, Value, Compare >::operator=(BSTree &&other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    comp_ = std::move(other.comp_);
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template < class Key, class Value, class Compare >
bool karpovich::BSTree< Key, Value, Compare >::empty() const
{
  return !size_;
}

template < class Key, class Value, class Compare >
size_t karpovich::BSTree< Key, Value, Compare >::size() const
{
  return size_;
}

template < class Key, class Value, class Compare >
karpovich::TreeNode< Key, Value > *karpovich::BSTree< Key, Value, Compare >::clone(TreeNode< Key, Value > *src,
                                                                                   TreeNode< Key, Value > *parent)
{
  if (src == nullptr) {
    return nullptr;
  }
  TreeNode< Key, Value > *n = new TreeNode< Key, Value >(src->key_, src->value_, parent);
  n->left_ = clone(src->left_, n);
  n->right_ = clone(src->right_, n);
  return n;
}

template < class Key, class Value, class Compare >
karpovich::TreeNode< Key, Value > *karpovich::BSTree< Key, Value, Compare >::findNode(const Key &k) const
{
  TreeNode< Key, Value > *cur = root_;
  while (cur != nullptr) {
    if (comp_(k, cur->key_)) {
      cur = cur->left_;
    } else if (comp_(cur->key_, k)) {
      cur = cur->right_;
    } else {
      return cur;
    }
  }
  return nullptr;
}

template < class Key, class Value, class Compare >
const Value &karpovich::BSTree< Key, Value, Compare >::at(const Key &k) const
{
  TreeNode< Key, Value > *n = findNode(k);
  if (n == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return n->value_;
}

template < class Key, class Value, class Compare >
Value &karpovich::BSTree< Key, Value, Compare >::at(const Key &k)
{
  TreeNode< Key, Value > *n = findNode(k);
  if (n == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return n->value_;
}

template < class Key, class Value, class Compare >
void karpovich::BSTree< Key, Value, Compare >::push(const Key &k, const Value &v)
{
  if (root_ == nullptr) {
    root_ = new TreeNode< Key, Value >(k, v, nullptr);
    ++size_;
    return;
  }
  TreeNode< Key, Value > *cur = root_;
  while (true) {
    if (comp_(k, cur->key_)) {
      if (cur->left_ == nullptr) {
        cur->left_ = new TreeNode< Key, Value >(k, v, cur);
        ++size_;
        return;
      }
      cur = cur->left_;
    } else if (comp_(cur->key_, k)) {
      if (cur->right_ == nullptr) {
        cur->right_ = new TreeNode< Key, Value >(k, v, cur);
        ++size_;
        return;
      }
      cur = cur->right_;
    } else {
      cur->value_ = v;
      return;
    }
  }
}

template < class Key, class Value, class Compare >
void karpovich::BSTree< Key, Value, Compare >::push(Key &&k, Value &&v)
{
  if (root_ == nullptr) {
    root_ = new TreeNode< Key, Value >(std::move(k), std::move(v), nullptr);
    ++size_;
    return;
  }
  TreeNode< Key, Value > *cur = root_;
  while (true) {
    if (comp_(k, cur->key_)) {
      if (cur->left_ == nullptr) {
        cur->left_ = new TreeNode< Key, Value >(std::move(k), std::move(v), cur);
        ++size_;
        return;
      }
      cur = cur->left_;
    } else if (comp_(cur->key_, k)) {
      if (cur->right_ == nullptr) {
        cur->right_ = new TreeNode< Key, Value >(std::move(k), std::move(v), cur);
        ++size_;
        return;
      }
      cur = cur->right_;
    } else {
      cur->value_ = std::move(v);
      return;
    }
  }
}

template < class Key, class Value, class Compare >
Value karpovich::BSTree< Key, Value, Compare >::drop(const Key &k)
{
  TreeNode< Key, Value > *node = findNode(k);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  Value res = std::move(node->value_);
  if (node->left_ && node->right_) {
    TreeNode< Key, Value > *succ = fallLeft(node->right_);
    node->key_ = std::move(succ->key_);
    node->value_ = std::move(succ->value_);
    node = succ;
  }
  TreeNode< Key, Value > *child = (node->left_) ? node->left_ : node->right_;
  if (child) {
    child->parent_ = node->parent_;
  }
  if (!node->parent_) {
    root_ = child;
  } else if (node->parent_->left_ == node) {
    node->parent_->left_ = child;
  } else {
    node->parent_->right_ = child;
  }
  delete node;
  --size_;
  return res;
}

template < class Key, class Value, class Compare >
void karpovich::BSTree< Key, Value, Compare >::clearImpl(TreeNode< Key, Value > *node) noexcept
{
  if (node == nullptr) {
    return;
  }
  clearImpl(node->left_);
  clearImpl(node->right_);
  delete node;
}

template < class Key, class Value, class Compare >
void karpovich::BSTree< Key, Value, Compare >::clear() noexcept
{
  clearImpl(root_);
  root_ = nullptr;
  size_ = 0;
}

template < class Key, class Value, class Compare >
void karpovich::BSTree< Key, Value, Compare >::swap(BSTree &other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template < class Key, class Value, class Compare >
karpovich::TreeNode< Key, Value > *
karpovich::BSTree< Key, Value, Compare >::fallLeft(TreeNode< Key, Value > *node) const
{
  while (node != nullptr && node->left_ != nullptr) {
    node = node->left_;
  }
  return node;
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::iterator karpovich::BSTree< Key, Value, Compare >::begin()
{
  return iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::iterator karpovich::BSTree< Key, Value, Compare >::end() noexcept
{
  return iterator(nullptr);
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::begin() const
{
  return const_iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::end() const noexcept
{
  return const_iterator(nullptr);
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  return const_iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return const_iterator(nullptr);
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
{
  TreeNode< Key, Value > *x = static_cast< TreeNode< Key, Value > * >(it.node_);
  if (x == nullptr || x->right_ == nullptr) {
    return it;
  }
  TreeNode< Key, Value > *y = x->right_;
  x->right_ = y->left_;
  if (y->left_ != nullptr) {
    y->left_->parent_ = x;
  }
  y->parent_ = x->parent_;
  if (x->parent_ == nullptr) {
    root_ = y;
  } else if (x == x->parent_->left_) {
    x->parent_->left_ = y;
  } else {
    x->parent_->right_ = y;
  }
  y->left_ = x;
  x->parent_ = y;
  return const_iterator(y);
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
{
  TreeNode< Key, Value > *y = static_cast< TreeNode< Key, Value > * >(it.node_);
  if (y == nullptr || y->left_ == nullptr) {
    return it;
  }
  TreeNode< Key, Value > *x = y->left_;
  y->left_ = x->right_;
  if (x->right_ != nullptr) {
    x->right_->parent_ = y;
  }
  x->parent_ = y->parent_;
  if (y->parent_ == nullptr) {
    root_ = x;
  } else if (y == y->parent_->left_) {
    y->parent_->left_ = x;
  } else {
    y->parent_->right_ = x;
  }
  x->right_ = y;
  y->parent_ = x;
  return const_iterator(x);
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
{
  TreeNode< Key, Value > *node = static_cast< TreeNode< Key, Value > * >(it.node_);
  if (node == nullptr || node->left_ == nullptr || node->left_->right_ == nullptr) {
    return it;
  }
  rotateRight(const_iterator(node->left_));
  return rotateLeft(it);
}

template < class Key, class Value, class Compare >
typename karpovich::BSTree< Key, Value, Compare >::const_iterator
karpovich::BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
{
  TreeNode< Key, Value > *node = static_cast< TreeNode< Key, Value > * >(it.node_);
  if (node == nullptr || node->right_ == nullptr || node->right_->left_ == nullptr) {
    return it;
  }
  rotateLeft(const_iterator(node->right_));
  return rotateRight(it);
}

template < class Key, class Value, class Compare >
size_t karpovich::BSTree< Key, Value, Compare >::calcHeight(TreeNode< Key, Value > *node) const
{
  if (node == nullptr) {
    return 0;
  }
  size_t l = calcHeight(node->left_);
  size_t r = calcHeight(node->right_);
  return 1 + ((l > r) ? l : r);
}

template < class Key, class Value, class Compare >
size_t karpovich::BSTree< Key, Value, Compare >::height() const
{
  return calcHeight(root_);
}

template < class Key, class Value, class Compare >
size_t karpovich::BSTree< Key, Value, Compare >::height(const_iterator it) const
{
  return calcHeight(static_cast< TreeNode< Key, Value > * >(it.node_));
}

#endif
