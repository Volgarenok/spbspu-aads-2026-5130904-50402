#ifndef RBIT_HPP
#define RBIT_HPP

#include "TreeNode.hpp"

namespace khalikov
{
  template < class T, class Cmp > struct RBIt
  {
    RBIt();
    explicit RBIt(TreeNode< T, Cmp > *node);

    T &operator*();
    T *operator->();

    bool operator==(const RBIt &rhs) const noexcept;
    bool operator!=(const RBIt &rhs) const noexcept;

    RBIt &operator++();
    RBIt operator++(int);

  private:
    TreeNode< T, Cmp > *curr;
  };
}

template < class T, class Cmp >
khalikov::RBIt< T, Cmp >::RBIt():
  curr(nullptr)
{}

template < class T, class Cmp >
khalikov::RBIt< T, Cmp >::RBIt(TreeNode< T, Cmp > *node):
  curr(node)
{}

template < class T, class Cmp > T &khalikov::RBIt< T, Cmp >::operator*()
{
  return curr->data;
}

template < class T, class Cmp > T *khalikov::RBIt< T, Cmp >::operator->()
{
  return &(curr->data);
}

template < class T, class Cmp > bool khalikov::RBIt< T, Cmp >::operator==(const RBIt &rhs) const noexcept
{
  return curr == rhs.curr;
}

template < class T, class Cmp > bool khalikov::RBIt< T, Cmp >::operator!=(const RBIt &rhs) const noexcept
{
  return curr != rhs.curr;
}

template < class T, class Cmp > khalikov::RBIt< T, Cmp > &khalikov::RBIt< T, Cmp >::operator++()
{
  if (!curr) {
    return *this;
  }
  if (curr->right) {
    curr = curr->right;
    while (curr->left) {
      curr = curr->left;
    }
  } else {
    TreeNode< T, Cmp > *p = curr->parent;
    while (p && curr == p->right) {
      curr = p;
      p = p->parent;
    }
    curr = p;
  }
  return *this;
}

template < class T, class Cmp > khalikov::RBIt< T, Cmp > khalikov::RBIt< T, Cmp >::operator++(int)
{
  RBIt temp = *this;
  ++(*this);
  return temp;
}

#endif
