#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <cstddef>

namespace lavrentev
{
  template< class Key, class Value >
  class BSTIterator {

  };

  template< class Key, class Value >
  class BSTConstIterator {

  };

  template< class Key, class Value, class Compare >
  class BSTree {

  public:
    void push(Key k, Value v);
    Value get(Key k) const;
    void drop(Key k);

    using const_iterator = BSTConstIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;

  private:
    struct Node
    {
      Key key_;
      Value value_;
      Node *left_ = nullptr;
      Node *right_ = nullptr;
    };
    Node *fakeroot_;
    Compare compare_;
  };
}

#endif
