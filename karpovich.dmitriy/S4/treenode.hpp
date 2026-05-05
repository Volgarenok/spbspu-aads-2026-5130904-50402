#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <utility>

namespace karpovich
{
  template < class Key, class Value >
  struct TreeNode
  {
    std::pair< const Key, Value > data_;
    TreeNode *left_;
    TreeNode *right_;
    TreeNode *parent_;
  };
}

#endif
