#ifndef BITREE_HPP
#define BITREE_HPP

#include <utility>

namespace afanasev
{
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

    BSTree() : root_(& sentinel_) {}

    ~BSTree();

    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);

    using const_iterator = BSTConstIterator< Key, Value >;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();






  private:
    NodeBiTree< Key, Value > sentinel_;
    NodeBiTree< Key, Value > * root_;
    size_t size_;
    Compare comp_;
  };
}

#endif
