#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <algorithm>
#include <functional>
#include <stdexcept>

#include "bstiterators.hpp"

namespace sogdanov {

  template< class Key, class Value, class Compare = std::less< Key >, bool Multi = false >
  class BSTree {
  public:
    using node_type = Node< Key, Value >;
    using node_ptr = node_type *;

    BSTree();
    BSTree(const BSTree< Key, Value, Compare, Multi > &other);
    ~BSTree();
    BSTree< Key, Value, Compare, Multi > &operator=(const BSTree< Key, Value, Compare, Multi > &other);

    bool empty() const;
    size_t size() const;

    BSTIterator< Key, Value > begin();
    BSTIterator< Key, Value > end();
    BSTConstIterator< Key, Value > begin() const;
    BSTConstIterator< Key, Value > end() const;

    BSTIterator< Key, Value > find(const Key &k);
    BSTConstIterator< Key, Value > find(const Key &k) const;

    void push(const Key &k, const Value &v);
    Value &get(const Key &k);
    const Value &get(const Key &k) const;
    void drop(const Key &k);

    BSTConstIterator< Key, Value > rotateLeft(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateRight(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateLargeLeft(BSTConstIterator< Key, Value > it);
    BSTConstIterator< Key, Value > rotateLargeRight(BSTConstIterator< Key, Value > it);

    size_t height(BSTConstIterator< Key, Value > it) const;
    size_t height() const;

  protected:
    node_ptr root;
    node_ptr fake_leaf;
    Compare comp;
    size_t tree_size;

    void swap(BSTree< Key, Value, Compare, Multi > &other) noexcept;
    void destroy(node_ptr n);
    node_ptr copyTree(const node_type *otherNode, node_ptr parent, node_ptr otherFake);
    size_t calculateHeight(node_ptr n) const;

    node_ptr pushInternal(const Key &k, const Value &v);
    node_ptr dropInternal(const Key &k);
  };

  template< class Key, class Value, class Compare, bool Multi >
  void BSTree< Key, Value, Compare, Multi >::swap(BSTree< Key, Value, Compare, Multi > &other) noexcept
  {
    std::swap(root, other.root);
    std::swap(fake_leaf, other.fake_leaf);
    std::swap(tree_size, other.tree_size);
    std::swap(comp, other.comp);
  }

  template< class Key, class Value, class Compare, bool Multi >
  void BSTree< Key, Value, Compare, Multi >::destroy(node_ptr n)
  {
    if (n != fake_leaf) {
      destroy(n->left);
      destroy(n->right);
      delete n;
    }
  }

  template< class Key, class Value, class Compare, bool Multi >
  typename BSTree< Key, Value, Compare, Multi >::node_ptr
  BSTree< Key, Value, Compare, Multi >::copyTree(const node_type *otherNode, node_ptr parent, node_ptr otherFake)
  {
    if (otherNode == otherFake) {
      return fake_leaf;
    }
    node_ptr newNode = new node_type(otherNode->data.first, otherNode->data.second, parent);
    try {
      newNode->height = otherNode->height;
      newNode->left = fake_leaf;
      newNode->right = fake_leaf;
      newNode->left = copyTree(otherNode->left, newNode, otherFake);
      newNode->right = copyTree(otherNode->right, newNode, otherFake);
    } catch (...) {
      destroy(newNode);
      throw;
    }
    return newNode;
  }

  template< class Key, class Value, class Compare, bool Multi >
  size_t BSTree< Key, Value, Compare, Multi >::calculateHeight(node_ptr n) const
  {
    if (n == fake_leaf) {
      return 0;
    }
    return 1 + std::max(calculateHeight(n->left), calculateHeight(n->right));
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTree< Key, Value, Compare, Multi >::BSTree():
    tree_size(0)
  {
    fake_leaf = new node_type(Key(), Value(), nullptr);
    fake_leaf->left = fake_leaf;
    fake_leaf->right = fake_leaf;
    fake_leaf->height = 0;
    root = fake_leaf;
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTree< Key, Value, Compare, Multi >::BSTree(const BSTree< Key, Value, Compare, Multi > &other):
    comp(other.comp),
    tree_size(other.tree_size)
  {
    fake_leaf = new node_type(Key(), Value(), nullptr);
    fake_leaf->left = fake_leaf;
    fake_leaf->right = fake_leaf;
    fake_leaf->height = 0;
    try {
      root = copyTree(other.root, nullptr, other.fake_leaf);
    } catch (...) {
      delete fake_leaf;
      throw;
    }
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTree< Key, Value, Compare, Multi >::~BSTree()
  {
    destroy(root);
    delete fake_leaf;
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTree< Key, Value, Compare, Multi > &BSTree< Key, Value, Compare, Multi >::operator=(const BSTree< Key, Value, Compare, Multi > &other)
  {
    if (this != &other) {
      BSTree< Key, Value, Compare, Multi > tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class Value, class Compare, bool Multi >
  bool BSTree< Key, Value, Compare, Multi >::empty() const
  {
    return root == fake_leaf;
  }

  template< class Key, class Value, class Compare, bool Multi >
  size_t BSTree< Key, Value, Compare, Multi >::size() const
  {
    return tree_size;
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::begin()
  {
    node_ptr curr = root;
    while (curr != fake_leaf && curr->left != fake_leaf) {
      curr = curr->left;
    }
    return BSTIterator< Key, Value >(curr, fake_leaf);
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::end()
  {
    return BSTIterator< Key, Value >(fake_leaf, fake_leaf);
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::begin() const
  {
    const node_type *curr = root;
    while (curr != fake_leaf && curr->left != fake_leaf) {
      curr = curr->left;
    }
    return BSTConstIterator< Key, Value >(curr, fake_leaf);
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::end() const
  {
    return BSTConstIterator< Key, Value >(fake_leaf, fake_leaf);
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::find(const Key &k)
  {
    node_ptr curr = root;
    while (curr != fake_leaf) {
      if (comp(k, curr->data.first)) {
        curr = curr->left;
      } else if (comp(curr->data.first, k)) {
        curr = curr->right;
      } else {
        return BSTIterator< Key, Value >(curr, fake_leaf);
      }
    }
    return end();
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::find(const Key &k) const
  {
    node_ptr curr = root;
    while (curr != fake_leaf) {
      if (comp(k, curr->data.first)) {
        curr = curr->left;
      } else if (comp(curr->data.first, k)) {
        curr = curr->right;
      } else {
        return BSTConstIterator< Key, Value >(curr, fake_leaf);
      }
    }
    return end();
  }

  template< class Key, class Value, class Compare, bool Multi >
  typename BSTree< Key, Value, Compare, Multi >::node_ptr
  BSTree< Key, Value, Compare, Multi >::pushInternal(const Key &k, const Value &v)
  {
    if (root == fake_leaf) {
      root = new node_type(k, v, nullptr);
      root->left = fake_leaf;
      root->right = fake_leaf;
      tree_size++;
      return root;
    }
    node_ptr curr = root;
    node_ptr parent = nullptr;
    while (curr != fake_leaf) {
      parent = curr;
      if (comp(k, curr->data.first)) {
        curr = curr->left;
      } else if (comp(curr->data.first, k)) {
        curr = curr->right;
      } else {
        if (!Multi) {
          curr->data.second = v;
          return curr;
        } else {
          curr = curr->right;
        }
      }
    }
    node_ptr newNode = new node_type(k, v, parent);
    newNode->left = fake_leaf;
    newNode->right = fake_leaf;
    if (comp(k, parent->data.first)) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
    tree_size++;
    return newNode;
  }

  template< class Key, class Value, class Compare, bool Multi >
  void BSTree< Key, Value, Compare, Multi >::push(const Key &k, const Value &v)
  {
    pushInternal(k, v);
  }

  template< class Key, class Value, class Compare, bool Multi >
  Value &BSTree< Key, Value, Compare, Multi >::get(const Key &k)
  {
    BSTIterator< Key, Value > it = find(k);
    if (it != end()) {
      return (*it).second;
    }
    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Compare, bool Multi >
  const Value &BSTree< Key, Value, Compare, Multi >::get(const Key &k) const
  {
    BSTConstIterator< Key, Value > it = find(k);
    if (it != end()) {
      return (*it).second;
    }
    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Compare, bool Multi >
  typename BSTree< Key, Value, Compare, Multi >::node_ptr
  BSTree< Key, Value, Compare, Multi >::dropInternal(const Key &k)
  {
    BSTIterator< Key, Value > it = find(k);
    if (it == end()) {
      return nullptr;
    }

    node_ptr node = it.node;
    node_ptr balance_start = nullptr;

    if (node->left == fake_leaf && node->right == fake_leaf) {
      balance_start = node->parent;
      if (node->parent != nullptr) {
        if (node->parent->left == node) {
          node->parent->left = fake_leaf;
        } else {
          node->parent->right = fake_leaf;
        }
      } else {
        root = fake_leaf;
      }
      delete node;
    } else if (node->left == fake_leaf || node->right == fake_leaf) {
      node_ptr child = (node->left != fake_leaf) ? node->left : node->right;
      child->parent = node->parent;
      balance_start = node->parent;
      if (node->parent != nullptr) {
        if (node->parent->left == node) {
          node->parent->left = child;
        } else {
          node->parent->right = child;
        }
      } else {
        root = child;
      }
      delete node;
    } else {
      node_ptr succ = node->right;
      while (succ->left != fake_leaf) {
        succ = succ->left;
      }
      node->data = succ->data;
      node_ptr child = succ->right;
      balance_start = succ->parent;
      if (succ->parent->left == succ) {
        succ->parent->left = child;
      } else {
        succ->parent->right = child;
      }
      if (child != fake_leaf) {
        child->parent = succ->parent;
      }
      if (balance_start == succ) {
        balance_start = node;
      }
      delete succ;
    }
    tree_size--;
    return balance_start;
  }

  template< class Key, class Value, class Compare, bool Multi >
  void BSTree< Key, Value, Compare, Multi >::drop(const Key &k)
  {
    dropInternal(k);
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::rotateLeft(BSTConstIterator< Key, Value > it)
  {
    node_ptr node = const_cast< node_ptr >(it.node);
    if (node == fake_leaf || node->parent == nullptr || node->parent->right != node) {
      return it;
    }

    node_ptr parent = node->parent;
    node_ptr grand = parent->parent;

    parent->right = node->left;
    if (node->left != fake_leaf) {
      node->left->parent = parent;
    }

    node->left = parent;
    parent->parent = node;

    node->parent = grand;
    if (grand != nullptr) {
      if (grand->left == parent) {
        grand->left = node;
      } else {
        grand->right = node;
      }
    } else {
      root = node;
    }
    return BSTConstIterator< Key, Value >(node, fake_leaf);
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::rotateRight(BSTConstIterator< Key, Value > it)
  {
    node_ptr node = const_cast< node_ptr >(it.node);
    if (node == fake_leaf || node->parent == nullptr || node->parent->left != node) {
      return it;
    }

    node_ptr parent = node->parent;
    node_ptr grand = parent->parent;

    parent->left = node->right;
    if (node->right != fake_leaf) {
      node->right->parent = parent;
    }

    node->right = parent;
    parent->parent = node;

    node->parent = grand;
    if (grand != nullptr) {
      if (grand->left == parent) {
        grand->left = node;
      } else {
        grand->right = node;
      }
    } else {
      root = node;
    }
    return BSTConstIterator< Key, Value >(node, fake_leaf);
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::rotateLargeLeft(BSTConstIterator< Key, Value > it)
  {
    node_ptr node = const_cast< node_ptr >(it.node);
    if (node == fake_leaf || node->parent == nullptr) {
      return it;
    }
    rotateRight(BSTConstIterator< Key, Value >(node, fake_leaf));
    return rotateLeft(BSTConstIterator< Key, Value >(node, fake_leaf));
  }

  template< class Key, class Value, class Compare, bool Multi >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare, Multi >::rotateLargeRight(BSTConstIterator< Key, Value > it)
  {
    node_ptr node = const_cast< node_ptr >(it.node);
    if (node == fake_leaf || node->parent == nullptr) {
      return it;
    }
    rotateLeft(BSTConstIterator< Key, Value >(node, fake_leaf));
    return rotateRight(BSTConstIterator< Key, Value >(node, fake_leaf));
  }

  template< class Key, class Value, class Compare, bool Multi >
  size_t BSTree< Key, Value, Compare, Multi >::height(BSTConstIterator< Key, Value > it) const
  {
    return calculateHeight(const_cast< node_ptr >(it.node));
  }

  template< class Key, class Value, class Compare, bool Multi >
  size_t BSTree< Key, Value, Compare, Multi >::height() const
  {
    return calculateHeight(root);
  }

}

#endif
