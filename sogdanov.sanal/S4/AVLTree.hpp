#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "bstree.hpp"

namespace sogdanov {

  template< class Key, class Value, class Compare = std::less< Key >, bool Multi = false >
  class AVLTree : public BSTree< Key, Value, Compare, Multi > {
  public:
    BSTIterator< Key, Value > push(const Key &k, const Value &v);
    void drop(const Key &k);

  private:
    size_t getHeight(Node< Key, Value > *n) const;
    void fixHeight(Node< Key, Value > *n);
    int bFactor(Node< Key, Value > *n) const;
    Node< Key, Value > *rotateLeftAVL(Node< Key, Value > *q);
    Node< Key, Value > *rotateRightAVL(Node< Key, Value > *p);
    void balanceUp(Node< Key, Value > *n);
  };

  template< class Key, class Value, class Compare, bool Multi >
  BSTIterator< Key, Value > AVLTree< Key, Value, Compare, Multi >::push(const Key &k, const Value &v)
  {
    Node< Key, Value > *n = this->pushInternal(k, v);
    balanceUp(n);
    return BSTIterator< Key, Value >(n, this->fake_leaf);
  }

  template< class Key, class Value, class Compare, bool Multi >
  void AVLTree< Key, Value, Compare, Multi >::drop(const Key &k)
  {
    if (!Multi) {
      Node< Key, Value > *n = this->dropInternal(k);
      if (n != nullptr) {
        balanceUp(n);
      }
    } else {
      Node< Key, Value > *n = this->dropInternal(k);
      while (n != nullptr) {
        balanceUp(n);
        n = this->dropInternal(k);
      }
    }
  }

  template< class Key, class Value, class Compare, bool Multi >
  size_t AVLTree< Key, Value, Compare, Multi >::getHeight(Node< Key, Value > *n) const
  {
    if (n == this->fake_leaf) {
      return 0;
    }
    return n->height;
  }

  template< class Key, class Value, class Compare, bool Multi >
  void AVLTree< Key, Value, Compare, Multi >::fixHeight(Node< Key, Value > *n)
  {
    if (n != this->fake_leaf) {
      size_t hl = getHeight(n->left);
      size_t hr = getHeight(n->right);
      n->height = (hl > hr ? hl : hr) + 1;
    }
  }

  template< class Key, class Value, class Compare, bool Multi >
  int AVLTree< Key, Value, Compare, Multi >::bFactor(Node< Key, Value > *n) const
  {
    if (n == this->fake_leaf) {
      return 0;
    }
    return static_cast< int >(getHeight(n->right)) - static_cast< int >(getHeight(n->left));
  }

  template< class Key, class Value, class Compare, bool Multi >
  Node< Key, Value > *AVLTree< Key, Value, Compare, Multi >::rotateLeftAVL(Node< Key, Value > *q)
  {
    if (q == this->fake_leaf || q->right == this->fake_leaf) {
      return q;
    }
    Node< Key, Value > *p = q->right;
    q->right = p->left;
    if (p->left != this->fake_leaf) {
      p->left->parent = q;
    }
    p->parent = q->parent;
    if (q->parent == nullptr) {
      this->root = p;
    } else if (q == q->parent->left) {
      q->parent->left = p;
    } else {
      q->parent->right = p;
    }
    p->left = q;
    q->parent = p;
    fixHeight(q);
    fixHeight(p);
    return p;
  }

  template< class Key, class Value, class Compare, bool Multi >
  Node< Key, Value > *AVLTree< Key, Value, Compare, Multi >::rotateRightAVL(Node< Key, Value > *p)
  {
    if (p == this->fake_leaf || p->left == this->fake_leaf) {
      return p;
    }
    Node< Key, Value > *q = p->left;
    p->left = q->right;
    if (q->right != this->fake_leaf) {
      q->right->parent = p;
    }
    q->parent = p->parent;
    if (p->parent == nullptr) {
      this->root = q;
    } else if (p == p->parent->right) {
      p->parent->right = q;
    } else {
      p->parent->left = q;
    }
    q->right = p;
    p->parent = q;
    fixHeight(p);
    fixHeight(q);
    return q;
  }

  template< class Key, class Value, class Compare, bool Multi >
  void AVLTree< Key, Value, Compare, Multi >::balanceUp(Node< Key, Value > *n)
  {
    while (n != nullptr && n != this->fake_leaf) {
      fixHeight(n);
      int bf = bFactor(n);
      Node< Key, Value > *next = n->parent;

      if (bf == 2) {
        if (bFactor(n->right) < 0) {
          rotateRightAVL(n->right);
        }
        n = rotateLeftAVL(n);
        if (n->parent == nullptr) {
          this->root = n;
        }
      } else if (bf == -2) {
        if (bFactor(n->left) > 0) {
          rotateLeftAVL(n->left);
        }
        n = rotateRightAVL(n);
        if (n->parent == nullptr) {
          this->root = n;
        }
      }
      n = next;
    }
  }

}

#endif
