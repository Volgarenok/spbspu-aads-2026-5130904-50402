#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
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
    using TNode = TreeNode< Key, Value >;

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
    Value drop(const Key &k);

    void clear();
    void swap(BSTree &other);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

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
  root_(clone(other.root_, nullptr)),
  size_(other.size_),
  comp_(other.comp_)
{}

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
  return n->data_.second;
}

template < class Key, class Value, class Compare >
Value &karpovich::BSTree< Key, Value, Compare >::at(const Key &k)
{
  TreeNode< Key, Value > *n = findNode(k);
  if (n == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return n->data_.second;
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
    if (comp_(k, cur->data_.first)) {
      if (cur->left_ == nullptr) {
        cur->left_ = new TreeNode< Key, Value >(k, v, cur);
        ++size_;
        return;
      }
      cur = cur->left_;
    } else if (comp_(cur->data_.first, k)) {
      if (cur->right_ == nullptr) {
        cur->right_ = new TreeNode< Key, Value >(k, v, cur);
        ++size_;
        return;
      }
      cur = cur->right_;
    } else {
      cur->data_.second = v;
      return;
    }
  }
}

#endif
