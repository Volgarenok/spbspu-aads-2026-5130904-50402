#ifndef TREENODE_HPP
#define TREENODE_HPP

namespace khalikov
{
  template < class T, class Cmp > struct TreeNode
  {
    T data;
    TreeNode< T, Cmp > *parent;
    TreeNode< T, Cmp > *left;
    TreeNode< T, Cmp > *right;
    char color;
    Cmp cmp;

    TreeNode(const T &val);
  };
}

template < class T, class Cmp >
khalikov::TreeNode< T, Cmp >::TreeNode(const T &val):
  data(val),
  parent(nullptr),
  left(nullptr),
  right(nullptr),
  color('R'),
  cmp()
{}
#endif
