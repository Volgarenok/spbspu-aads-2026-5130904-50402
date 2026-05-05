#ifndef TREENODE_HPP
#define TREENODE_HPP

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
  };
}

#endif
