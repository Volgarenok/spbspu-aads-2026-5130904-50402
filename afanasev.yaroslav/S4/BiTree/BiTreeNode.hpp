#ifndef BITREE_NODE_HPP
#define BITREE_NODE_HPP

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

    NodeBiTree(const Key & key, Value && val, NodeBiTree * parent):
      key_(key),
      val_(std::move(val)),
      left_(nullptr),
      right_(nullptr),
      parent_(parent)
    {}

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
}

#endif
