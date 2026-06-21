#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <utility>
#include "TreeNode.hpp"
#include "RBIt.hpp"
#include "RBCIt.hpp"

namespace khalikov
{
  template < class T, class Cmp > struct RBTree
  {
    using it_t = khalikov::RBIt< T, Cmp >;
    using cit_t = khalikov::RBCIt< T, Cmp >;

    RBTree();
    explicit RBTree(Cmp custom);
    ~RBTree();
    RBTree(const RBTree &rhs);
    RBTree(RBTree &&rhs) noexcept;
    RBTree &operator=(const RBTree &rhs);
    RBTree &operator=(RBTree &&rhs) noexcept;

    void swap(RBTree &rhs) noexcept;
    void clear(TreeNode< T, Cmp > *node);
    bool empty() const noexcept;
    bool insert(const T &val);
    bool remove(const T &val);

    it_t begin() noexcept;
    it_t end() noexcept;

    cit_t cbegin() const noexcept;
    cit_t cend() const noexcept;

  private:
    TreeNode< T, Cmp > *root;
    Cmp cmp;

    TreeNode< T, Cmp > *copy(TreeNode< T, Cmp > *node, TreeNode< T, Cmp > *parent);
    void rotateLeft(TreeNode< T, Cmp > *x);
    void rotateRight(TreeNode< T, Cmp > *x);
    void fixInsert(TreeNode< T, Cmp > *x);
    const TreeNode< T, Cmp > *fullLeft(const TreeNode< T, Cmp > *node) const;
    TreeNode< T, Cmp > *fullLeft(TreeNode< T, Cmp > *node);
    void transplant(TreeNode< T, Cmp > *u, TreeNode< T, Cmp > *v);
    void fixDelete(TreeNode< T, Cmp > *x, TreeNode< T, Cmp > *xParent);
  };
}

template < class T, class Cmp >
khalikov::RBTree< T, Cmp >::RBTree(Cmp custom):
  root(nullptr),
  cmp(custom)
{}

template < class T, class Cmp >
typename khalikov::RBTree< T, Cmp >::cit_t khalikov::RBTree< T, Cmp >::cbegin() const noexcept
{
  return cit_t(fullLeft(root));
}

template < class T, class Cmp >
typename khalikov::RBTree< T, Cmp >::cit_t khalikov::RBTree< T, Cmp >::cend() const noexcept
{
  return cit_t();
}

template < class T, class Cmp > typename khalikov::RBTree< T, Cmp >::it_t khalikov::RBTree< T, Cmp >::begin() noexcept
{
  return it_t(fullLeft(root));
}

template < class T, class Cmp > typename khalikov::RBTree< T, Cmp >::it_t khalikov::RBTree< T, Cmp >::end() noexcept
{
  return it_t();
}

template < class T, class Cmp > bool khalikov::RBTree< T, Cmp >::insert(const T &val)
{
  TreeNode< T, Cmp > *z = new TreeNode< T, Cmp >(val);
  TreeNode< T, Cmp > *y = nullptr;
  TreeNode< T, Cmp > *x = root;
  while (x) {
    y = x;
    if (!cmp(z->data, x->data) && !cmp(x->data, z->data)) {
      delete z;
      return false;
    }
    if (cmp(z->data, x->data)) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if (!y) {
    root = z;
  } else if (cmp(z->data, y->data)) {
    y->left = z;
  } else {
    y->right = z;
  }
  fixInsert(z);
  return true;
}

template < class T, class Cmp > void khalikov::RBTree< T, Cmp >::fixInsert(TreeNode< T, Cmp > *x)
{
  while (x != root && x->parent->color == 'R') {
    TreeNode< T, Cmp > *p = x->parent;
    TreeNode< T, Cmp > *g = p->parent;
    if (p == g->left) {
      TreeNode< T, Cmp > *u = g->right;
      if (u && u->color == 'R') {
        p->color = 'B';
        u->color = 'B';
        g->color = 'R';
        x = g;
      } else {
        if (x == p->right) {
          x = p;
          rotateLeft(x);
          p = x->parent;
        }
        p->color = 'B';
        g->color = 'R';
        rotateRight(g);
      }
    } else {
      TreeNode< T, Cmp > *u = g->left;
      if (u && u->color == 'R') {
        p->color = 'B';
        u->color = 'B';
        g->color = 'R';
        x = g;
      } else {
        if (x == p->left) {
          x = p;
          rotateRight(x);
          p = x->parent;
        }
        p->color = 'B';
        g->color = 'R';
        rotateLeft(g);
      }
    }
  }
  root->color = 'B';
}

template < class T, class Cmp > bool khalikov::RBTree< T, Cmp >::remove(const T &val)
{
  TreeNode< T, Cmp > *z = root;
  while (z && (cmp(val, z->data) || cmp(z->data, val))) {
    z = cmp(val, z->data) ? z->left : z->right;
  }
  if (!z) {
    return false;
  }
  TreeNode< T, Cmp > *x = nullptr;
  TreeNode< T, Cmp > *xParent = nullptr;
  TreeNode< T, Cmp > *y = z;
  char yColor = y->color;
  if (!z->left) {
    x = z->right;
    xParent = z->parent;
    transplant(z, z->right);
  } else if (!z->right) {
    x = z->left;
    xParent = z->parent;
    transplant(z, z->left);
  } else {
    y = fullLeft(z->right);
    yColor = y->color;
    x = y->right;
    if (y->parent == z) {
      xParent = y;
    } else {
      xParent = y->parent;
      transplant(y, y->right);
      y->right = z->right;
      if (y->right) {
        y->right->parent = y;
      }
    }
    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  delete z;
  if (yColor == 'B') {
    fixDelete(x, xParent);
  }
  return true;
}

template < class T, class Cmp >
void khalikov::RBTree< T, Cmp >::fixDelete(TreeNode< T, Cmp > *x, TreeNode< T, Cmp > *xParent)
{
  while (x != root && (!x || x->color == 'B')) {
    if (x == xParent->left) {
      TreeNode< T, Cmp > *s = xParent->right;
      if (s && s->color == 'R') {
        s->color = 'B';
        xParent->color = 'R';
        rotateLeft(xParent);
        s = xParent->right;
      }
      if ((!s->left || s->left->color == 'B') && (!s->right || s->right->color == 'B')) {
        if (s) {
          s->color = 'R';
        }
        x = xParent;
        xParent = x->parent;
      } else {
        if (!s->right || s->right->color == 'B') {
          if (s->left) {
            s->left->color = 'B';
          }
          s->color = 'R';
          rotateRight(s);
          s = xParent->right;
        }
        if (s) {
          s->color = xParent->color;
        }
        xParent->color = 'B';
        if (s && s->right) {
          s->right->color = 'B';
        }
        rotateLeft(xParent);
        x = root;
      }
    } else {
      TreeNode< T, Cmp > *s = xParent->left;
      if (s && s->color == 'R') {
        s->color = 'B';
        xParent->color = 'R';
        rotateRight(xParent);
        s = xParent->left;
      }
      if ((!s->left || s->left->color == 'B') && (!s->right || s->right->color == 'B')) {
        if (s) {
          s->color = 'R';
        }
        x = xParent;
        xParent = x->parent;
      } else {
        if (!s->left || s->left->color == 'B') {
          if (s->right) {
            s->right->color = 'B';
          }
          s->color = 'R';
          rotateLeft(s);
          s = xParent->left;
        }
        if (s) {
          s->color = xParent->color;
        }
        xParent->color = 'B';
        if (s && s->left) {
          s->left->color = 'B';
        }
        rotateRight(xParent);
        x = root;
      }
    }
  }
  if (x) {
    x->color = 'B';
  }
}

template < class T, class Cmp >
khalikov::TreeNode< T, Cmp > *khalikov::RBTree< T, Cmp >::fullLeft(TreeNode< T, Cmp > *node)
{
  while (node && node->left) {
    node = node->left;
  }
  return node;
}

template < typename T, typename Cmp >
const khalikov::TreeNode< T, Cmp > *khalikov::RBTree< T, Cmp >::fullLeft(const TreeNode< T, Cmp > *node) const
{
  while (node && node->left) {
    node = node->left;
  }
  return node;
}

template < class T, class Cmp >
void khalikov::RBTree< T, Cmp >::transplant(TreeNode< T, Cmp > *u, TreeNode< T, Cmp > *v)
{
  if (!u->parent) {
    root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  if (v) {
    v->parent = u->parent;
  }
}

template < class T, class Cmp > void khalikov::RBTree< T, Cmp >::rotateRight(TreeNode< T, Cmp > *x)
{
  TreeNode< T, Cmp > *y = x->left;
  x->left = y->right;
  if (y->right) {
    y->right->parent = x;
  }
  transplant(x, y);
  y->right = x;
  x->parent = y;
}

template < class T, class Cmp > void khalikov::RBTree< T, Cmp >::rotateLeft(TreeNode< T, Cmp > *x)
{
  TreeNode< T, Cmp > *y = x->right;
  x->right = y->left;
  if (y->left) {
    y->left->parent = x;
  }
  transplant(x, y);
  y->left = x;
  x->parent = y;
}

template < class T, class Cmp >
khalikov::RBTree< T, Cmp >::RBTree():
  root(nullptr),
  cmp()
{}

template < class T, class Cmp > khalikov::RBTree< T, Cmp >::~RBTree()
{
  clear(root);
  root = nullptr;
}

template < class T, class Cmp >
khalikov::RBTree< T, Cmp >::RBTree(const RBTree &rhs):
  root(nullptr),
  cmp(rhs.cmp)
{
  root = copy(rhs.root, nullptr);
}

template < class T, class Cmp >
khalikov::RBTree< T, Cmp >::RBTree(RBTree &&rhs) noexcept:
  root(nullptr),
  cmp(std::move(rhs.cmp))
{
  swap(rhs);
}

template < class T, class Cmp > khalikov::RBTree< T, Cmp > &khalikov::RBTree< T, Cmp >::operator=(const RBTree &rhs)
{
  if (this != std::addressof(rhs)) {
    RBTree temp(rhs);
    swap(temp);
  }
  return *this;
}

template < class T, class Cmp > khalikov::RBTree< T, Cmp > &khalikov::RBTree< T, Cmp >::operator=(RBTree &&rhs) noexcept
{
  if (this != std::addressof(rhs)) {
    clear(root);
    root = nullptr;
    swap(rhs);
  }
  return *this;
}

template < class T, class Cmp > void khalikov::RBTree< T, Cmp >::clear(TreeNode< T, Cmp > *node)
{
  if (!node) {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node;
}

template < class T, class Cmp > bool khalikov::RBTree< T, Cmp >::empty() const noexcept
{
  return root == nullptr;
}

template < class T, class Cmp >
khalikov::TreeNode< T, Cmp > *khalikov::RBTree< T, Cmp >::copy(TreeNode< T, Cmp > *node, TreeNode< T, Cmp > *parent)
{
  if (!node) {
    return nullptr;
  }
  TreeNode< T, Cmp > *newNode = new TreeNode< T, Cmp >(node->data);
  newNode->parent = parent;
  newNode->color = node->color;
  newNode->left = copy(node->left, newNode);
  newNode->right = copy(node->right, newNode);
  return newNode;
}

template < class T, class Cmp > void khalikov::RBTree< T, Cmp >::swap(RBTree &rhs) noexcept
{
  std::swap(root, rhs.root);
  std::swap(cmp, rhs.cmp);
}

#endif
