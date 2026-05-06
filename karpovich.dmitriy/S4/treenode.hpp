#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <utility>

namespace karpovich
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
}

template < class Key, class Value >
karpovich::TreeNode< Key, Value >::TreeNode(const Key &key, const Value &value,
                                            karpovich::TreeNode< Key, Value > *parent):
  key_(key),
  value_(value),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template < class Key, class Value >
karpovich::TreeNode< Key, Value >::TreeNode(Key &&key, Value &&value, karpovich::TreeNode< Key, Value > *parent):
  key_(std::move(key)),
  value_(std::move(value)),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

#endif
