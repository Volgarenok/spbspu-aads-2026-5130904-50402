#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
namespace dirko
{
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
    size_t size_;
    Compare comp_;
    Key key_;
    Value value_;
    BSTree< Key, Value, Compare > *left_;
    BSTree< Key, Value, Compare > *right_;
    BSTree< Key, Value, Compare > *parent_;
  };
}
#endif
