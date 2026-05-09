#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
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
  };
  template < class Key, class Value >
  class BSTConstIterator
  {
  };

  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    void push(Key k, Value v);
    Value get(Key k);
    void drop(Key k);

    size_t size() const noexcept;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();

    iterator begin();
    iterator end() noexcept;
    const_iterator cbegin() const noexcept;
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
#endif
