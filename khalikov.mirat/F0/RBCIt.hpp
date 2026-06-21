#ifndef RBCIT_HPP
#define RBCIT_HPP

#include "TreeNode.hpp"

namespace khalikov
{
  template < class T, class Cmp > struct RBCIt
  {
    RBCIt();
    explicit RBCIt(const TreeNode< T, Cmp > *node);

    const T &operator*() const;
    const T *operator->() const;

    bool operator==(const RBCIt &rhs) const noexcept;
    bool operator!=(const RBCIt &rhs) const noexcept;

    RBCIt &operator++();
    RBCIt operator++(int);

  private:
    const TreeNode< T, Cmp > *curr;
  };
}

template < class T, class Cmp >
khalikov::RBCIt< T, Cmp >::RBCIt():
  curr(nullptr)
{}

template < class T, class Cmp >
khalikov::RBCIt< T, Cmp >::RBCIt(const TreeNode< T, Cmp > *node):
  curr(node)
{}

template < class T, class Cmp > const T &khalikov::RBCIt< T, Cmp >::operator*() const
{
  return curr->data;
}

template < class T, class Cmp > const T *khalikov::RBCIt< T, Cmp >::operator->() const
{
  return &(curr->data);
}

template < class T, class Cmp > bool khalikov::RBCIt< T, Cmp >::operator==(const RBCIt &rhs) const noexcept
{
  return curr == rhs.curr;
}

template < class T, class Cmp > bool khalikov::RBCIt< T, Cmp >::operator!=(const RBCIt &rhs) const noexcept
{
  return curr != rhs.curr;
}

template < class T, class Cmp > khalikov::RBCIt< T, Cmp > &khalikov::RBCIt< T, Cmp >::operator++()
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

template < class T, class Cmp > khalikov::RBCIt< T, Cmp > khalikov::RBCIt< T, Cmp >::operator++(int)
{
  RBCIt temp = *this;
  ++(*this);
  return temp;
}

#endif
