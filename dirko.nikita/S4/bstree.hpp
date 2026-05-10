#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <utility>
namespace dirko
{
  template < class Key, class Value >
  struct TreeNode
  {
    Key key_;
    Value value_;
    TreeNode *left_;
    TreeNode *right_;
    TreeNode *parent_;

    TreeNode(const Key &key, const Value &value, TreeNode *parent);
    TreeNode(Key &&key, Value &&value, TreeNode *parent);
  };
  template < class Key, class Value >
  class BSTIterator
  {
    explicit BSTIterator(TreeNode< Key, Value > *node);
    std::pair< const Key, Value > operator*() const;
    BSTIterator &operator++();
    BSTIterator &operator--();
    BSTIterator operator++(int);
    BSTIterator operator--(int);
    bool operator==(const BSTIterator &other) const;
    bool operator!=(const BSTIterator &other) const;

    template < class K, class V, class Compare >
    friend class BSTree;

  private:
    TreeNode< Key, Value > *curr_;
  };
  template < class Key, class Value >
  class BSTConstIterator
  {
  public:
    explicit BSTConstIterator(TreeNode< Key, Value > *node);
    std::pair< const Key, Value > operator*() const;
    BSTConstIterator &operator++();
    BSTConstIterator &operator--();
    BSTConstIterator operator++(int);
    BSTConstIterator operator--(int);
    bool operator==(const BSTConstIterator &other) const;
    bool operator!=(const BSTConstIterator &other) const;

    template < class K, class V, class Compare >
    friend class BSTree;

  private:
    TreeNode< Key, Value > *curr_;
  };

  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other) noexcept;
    BSTree(std::initializer_list< std::pair< Key, Value > > il);
    ~BSTree();

    void push(const Key &k, const Value &v);
    Value get(Key k);
    void drop(Key k);

    size_t size() const noexcept;

    void swap(BSTree &other) noexcept;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();

    iterator begin();
    iterator end() noexcept;
    const_iterator cbegin() const;
    const_iterator cend() const noexcept;

  private:
    TreeNode< Key, Value > *root_;
    size_t size_;
    Compare comp_;
  };
}

template < class Key, class Value >
dirko::TreeNode< Key, Value >::TreeNode(const Key &key, const Value &value, TreeNode *parent):
  key_(key),
  value_(value),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}
template < class Key, class Value >
dirko::TreeNode< Key, Value >::TreeNode(Key &&key, Value &&value, TreeNode *parent):
  key_(std::move(key)),
  value_(std::move(value)),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template < class Key, class Value, class Compare >
dirko::BSTree< Key, Value, Compare >::BSTree():
  root_(static_cast< TreeNode< Key, Value > * >(::operator new(sizeof(TreeNode< Key, Value >)))),
  size_(0),
  comp_()
{}
template < class Key, class Value, class Compare >
dirko::BSTree< Key, Value, Compare >::BSTree(const BSTree &other):
  root_(static_cast< TreeNode< Key, Value > * >(::operator new(sizeof(TreeNode< Key, Value >)))),
  size_(0),
  comp_(other.comp_)
{ // TODO:
}
template < class Key, class Value, class Compare >
dirko::BSTree< Key, Value, Compare >::BSTree(BSTree &&other) noexcept:
  root_(other.root_),
  size_(other.size_),
  comp_(std::move(other.comp_))
{
  other.root_ = nullptr;
  other.size_ = 0;
}
template < class Key, class Value, class Compare >
dirko::BSTree< Key, Value, Compare >::BSTree(std::initializer_list< std::pair< Key, Value > > il):
  root_(static_cast< TreeNode< Key, Value > * >(::operator new(sizeof(TreeNode< Key, Value >)))),
  size_(0),
  comp_(Compare{})
{
  for (; size_ < il.size(); ++size_) {
    push(il[size_].first, il[size_].second);
  }
}

template < class Key, class Value >
dirko::BSTIterator< Key, Value >::BSTIterator(TreeNode< Key, Value > *node):
  curr_(node)
{}

template < class Key, class Value >
std::pair< const Key, Value > dirko::BSTIterator< Key, Value >::operator*() const
{
  return {curr_->key_, curr_->value_};
}

template < class Key, class Value >
dirko::BSTIterator< Key, Value > &dirko::BSTIterator< Key, Value >::operator++()
{
  if (!curr_) {
    return *this;
  }
  if (curr_->right_) {
    curr_ = curr_->right_;
    while (curr_->left_) {
      curr_ = curr_->left_;
    }
  } else {
    TreeNode< Key, Value > *parent = curr_->parent_;
    while (parent && curr_ == parent->right_) {
      curr_ = parent;
      parent = curr_->parent_;
    }
    curr_ = parent;
  }
  return *this;
}
template < class Key, class Value >
dirko::BSTIterator< Key, Value > dirko::BSTIterator< Key, Value >::operator++(int)
{
  BSTIterator ret = *this;
  ++(*this);
  return ret;
}
template < class Key, class Value >
dirko::BSTIterator< Key, Value > &dirko::BSTIterator< Key, Value >::operator--()
{
  if (!curr_) {
    return *this;
  }

  if (curr_->left_) {
    curr_ = curr_->left_;
    while (curr_->right_) {
      curr_ = curr_->right_;
    }
  } else {
    TreeNode< Key, Value > *parent = curr_->parent_;
    while (parent && curr_ == parent->left_) {
      curr_ = parent;
      parent = curr_->parent_;
    }
    curr_ = parent;
  }
  return *this;
}
template < class Key, class Value >
dirko::BSTIterator< Key, Value > dirko::BSTIterator< Key, Value >::operator--(int)
{
  BSTIterator ret = *this;
  --(*this);
  return ret;
}
template < class Key, class Value >
bool dirko::BSTIterator< Key, Value >::operator==(const BSTIterator &other) const
{
  return curr_ == other.curr_;
}

template < class Key, class Value >
bool dirko::BSTIterator< Key, Value >::operator!=(const BSTIterator &other) const
{
  return !(*this == other);
}

template < class Key, class Value >
dirko::BSTConstIterator< Key, Value >::BSTConstIterator(TreeNode< Key, Value > *node):
  curr_(node)
{}

template < class Key, class Value >
std::pair< const Key, Value > dirko::BSTConstIterator< Key, Value >::operator*() const
{
  return {curr_->key_, curr_->value_};
}

template < class Key, class Value >
dirko::BSTConstIterator< Key, Value > &dirko::BSTConstIterator< Key, Value >::operator++()
{
  if (!curr_) {
    return *this;
  }
  if (curr_->right_) {
    curr_ = curr_->right_;
    while (curr_->left_) {
      curr_ = curr_->left_;
    }
  } else {
    TreeNode< Key, Value > *parent = curr_->parent_;
    while (parent && curr_ == parent->right_) {
      curr_ = parent;
      parent = curr_->parent_;
    }
    curr_ = parent;
  }
  return *this;
}
template < class Key, class Value >
dirko::BSTConstIterator< Key, Value > dirko::BSTConstIterator< Key, Value >::operator++(int)
{
  BSTConstIterator ret = *this;
  ++(*this);
  return ret;
}
template < class Key, class Value >
dirko::BSTConstIterator< Key, Value > &dirko::BSTConstIterator< Key, Value >::operator--()
{
  if (!curr_) {
    return *this;
  }

  if (curr_->left_) {
    curr_ = curr_->left_;
    while (curr_->right_) {
      curr_ = curr_->right_;
    }
  } else {
    TreeNode< Key, Value > *parent = curr_->parent_;
    while (parent && curr_ == parent->left_) {
      curr_ = parent;
      parent = curr_->parent_;
    }
    curr_ = parent;
  }
  return *this;
}
template < class Key, class Value >
dirko::BSTConstIterator< Key, Value > dirko::BSTConstIterator< Key, Value >::operator--(int)
{
  BSTConstIterator ret = *this;
  --(*this);
  return ret;
}
template < class Key, class Value >
bool dirko::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator &other) const
{
  return curr_ == other.curr_;
}

template < class Key, class Value >
bool dirko::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator &other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Compare >
size_t dirko::BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template < class Key, class Value, class Compare >
void dirko::BSTree< Key, Value, Compare >::swap(BSTree &other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template < class Key, class Value, class Compare >
dirko::BSTIterator< Key, Value > dirko::BSTree< Key, Value, Compare >::begin()
{
  return iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
dirko::BSTIterator< Key, Value > dirko::BSTree< Key, Value, Compare >::end() noexcept
{
  return iterator(nullptr);
}
template < class Key, class Value, class Compare >
dirko::BSTConstIterator< Key, Value > dirko::BSTree< Key, Value, Compare >::cbegin() const
{
  return const_iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
dirko::BSTConstIterator< Key, Value > dirko::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return const_iterator(nullptr);
}
#endif
