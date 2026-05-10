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

    BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other) noexcept;
    BSTree(std::initializer_list< std::pair< Key, Value > > il);
    ~BSTree();

    void push(const Key &k, const Value &v);
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
#endif
