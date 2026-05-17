#ifndef BSTREE_HPP
#define BSTREE_HPP

#include "bstiterators.hpp"
#include <functional>
#include <stdexcept>
#include <algorithm>

namespace sogdanov
{

  template <class Key, class Value, class Compare = std::less<Key>>
  class BSTree
  {
  private:
    Node<Key, Value> *root;
    Node<Key, Value> *fake_leaf;
    Compare comp;
    size_t tree_size;

    void destroy(Node<Key, Value> *n);
    Node<Key, Value> *copyTree(const Node<Key, Value> *otherNode, Node<Key, Value> *parent, Node<Key, Value> *otherFake);
    size_t calculateHeight(Node<Key, Value> *n) const;

  public:
    BSTree();
    BSTree(const BSTree<Key, Value, Compare> &other);
    ~BSTree();
    BSTree<Key, Value, Compare> &operator=(const BSTree<Key, Value, Compare> &other);
    bool empty() const;

    BSTIterator<Key, Value> begin();
    BSTIterator<Key, Value> end();
    BSTConstIterator<Key, Value> begin() const;
    BSTConstIterator<Key, Value> end() const;

    BSTIterator<Key, Value> find(const Key &k);
    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);

    BSTConstIterator<Key, Value> rotateLeft(BSTConstIterator<Key, Value> it);
    BSTConstIterator<Key, Value> rotateRight(BSTConstIterator<Key, Value> it);
    BSTConstIterator<Key, Value> rotateLargeLeft(BSTConstIterator<Key, Value> it);
    BSTConstIterator<Key, Value> rotateLargeRight(BSTConstIterator<Key, Value> it);

    size_t height(BSTConstIterator<Key, Value> it) const;
    size_t height() const;
  };

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::destroy(Node<Key, Value> *n)
  {
    if (n != fake_leaf)
    {
      destroy(n->left);
      destroy(n->right);
      delete n;
    }
  }

  template <class Key, class Value, class Compare>
  Node<Key, Value> *BSTree<Key, Value, Compare>::copyTree(const Node<Key, Value> *otherNode, Node<Key, Value> *parent, Node<Key, Value> *otherFake)
  {
    if (otherNode == otherFake)
    {
      return fake_leaf;
    }
    Node<Key, Value> *newNode = new Node<Key, Value>(otherNode->data.first, otherNode->data.second, parent);
    newNode->left = copyTree(otherNode->left, newNode, otherFake);
    newNode->right = copyTree(otherNode->right, newNode, otherFake);
    return newNode;
  }

  template <class Key, class Value, class Compare>
  size_t BSTree<Key, Value, Compare>::calculateHeight(Node<Key, Value> *n) const
  {
    if (n == fake_leaf)
    {
      return 0;
    }
    return 1 + std::max(calculateHeight(n->left), calculateHeight(n->right));
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree() : tree_size(0)
  {
    fake_leaf = new Node<Key, Value>(Key(), Value(), nullptr);
    fake_leaf->left = fake_leaf;
    fake_leaf->right = fake_leaf;
    root = fake_leaf;
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree(const BSTree<Key, Value, Compare> &other) : tree_size(other.tree_size), comp(other.comp)
  {
    fake_leaf = new Node<Key, Value>(Key(), Value(), nullptr);
    fake_leaf->left = fake_leaf;
    fake_leaf->right = fake_leaf;
    root = copyTree(other.root, nullptr, other.fake_leaf);
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::~BSTree()
  {
    destroy(root);
    delete fake_leaf;
  }
  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare> &BSTree<Key, Value, Compare>::operator=(const BSTree<Key, Value, Compare> &other)
  {
    if (this != &other)
    {
      destroy(root);
      root = copyTree(other.root, nullptr, other.fake_leaf);
      tree_size = other.tree_size;
      comp = other.comp;
    }
    return *this;
  }

  template <class Key, class Value, class Compare>
  bool BSTree<Key, Value, Compare>::empty() const
  {
    return root == fake_leaf;
  }

  template <class Key, class Value, class Compare>
  BSTIterator<Key, Value> BSTree<Key, Value, Compare>::begin()
  {
    Node<Key, Value> *curr = root;
    while (curr != fake_leaf && curr->left != fake_leaf)
    {
      curr = curr->left;
    }
    return BSTIterator<Key, Value>(curr, fake_leaf);
  }

  template <class Key, class Value, class Compare>
  BSTIterator<Key, Value> BSTree<Key, Value, Compare>::end()
  {
    return BSTIterator<Key, Value>(fake_leaf, fake_leaf);
  }

  template <class Key, class Value, class Compare>
  BSTConstIterator<Key, Value> BSTree<Key, Value, Compare>::begin() const
  {
    const Node<Key, Value> *curr = root;
    while (curr != fake_leaf && curr->left != fake_leaf)
    {
      curr = curr->left;
    }
    return BSTConstIterator<Key, Value>(curr, fake_leaf);
  }

  template <class Key, class Value, class Compare>
  BSTConstIterator<Key, Value> BSTree<Key, Value, Compare>::end() const
  {
    return BSTConstIterator<Key, Value>(fake_leaf, fake_leaf);
  }
  template <class Key, class Value, class Compare>
  BSTIterator<Key, Value> BSTree<Key, Value, Compare>::find(const Key &k)
  {
    Node<Key, Value> *curr = root;
    while (curr != fake_leaf)
    {
      if (comp(k, curr->data.first))
      {
        curr = curr->left;
      }
      else if (comp(curr->data.first, k))
      {
        curr = curr->right;
      }
      else
      {
        return BSTIterator<Key, Value>(curr, fake_leaf);
      }
    }
    return end();
  }
  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::push(Key k, Value v)
  {
    if (root == fake_leaf)
    {
      root = new Node<Key, Value>(k, v, nullptr);
      root->left = fake_leaf;
      root->right = fake_leaf;
      tree_size++;
      return;
    }
    Node<Key, Value> *curr = root;
    Node<Key, Value> *parent = nullptr;
    while (curr != fake_leaf)
    {
      parent = curr;
      if (comp(k, curr->data.first))
      {
        curr = curr->left;
      }
      else if (comp(curr->data.first, k))
      {
        curr = curr->right;
      }
      else
      {
        curr->data.second = v;
        return;
      }
    }
    Node<Key, Value> *newNode = new Node<Key, Value>(k, v, parent);
    newNode->left = fake_leaf;
    newNode->right = fake_leaf;
    if (comp(k, parent->data.first))
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    tree_size++;
  }

  template <class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::get(Key k)
  {
    BSTIterator<Key, Value> it = find(k);
    if (it != end())
    {
      return (*it).second;
    }
    throw std::out_of_range("Key not found");
  }

  template <class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::drop(Key k)
  {
    BSTIterator<Key, Value> it = find(k);
    if (it == end())
    {
      throw std::out_of_range("Key not found");
    }
    Value val = (*it).second;

    Node<Key, Value> *node = it.node;
    if (node->left == fake_leaf && node->right == fake_leaf)
    {
      if (node->parent != nullptr)
      {
        if (node->parent->left == node)
        {
          node->parent->left = fake_leaf;
        }
        else
        {
          node->parent->right = fake_leaf;
        }
      }
      else
      {
        root = fake_leaf;
      }
      delete node;
    }
    else if (node->left == fake_leaf || node->right == fake_leaf)
    {
      Node<Key, Value> *child = (node->left != fake_leaf) ? node->left : node->right;
      child->parent = node->parent;
      if (node->parent != nullptr)
      {
        if (node->parent->left == node)
        {
          node->parent->left = child;
        }
        else
        {
          node->parent->right = child;
        }
      }
      else
      {
        root = child;
      }
      delete node;
    }
    else
    {
      Node<Key, Value> *succ = node->right;
      while (succ->left != fake_leaf)
      {
        succ = succ->left;
      }
      node->data = succ->data;
      Node<Key, Value> *child = succ->right;
      if (succ->parent->left == succ)
      {
        succ->parent->left = child;
      }
      else
      {
        succ->parent->right = child;
      }
      if (child != fake_leaf)
      {
        child->parent = succ->parent;
      }
      delete succ;
    }
    tree_size--;
    return val;
  }
   
  template <class Key, class Value, class Compare>
  BSTConstIterator<Key, Value> BSTree<Key, Value, Compare>::rotateLeft(BSTConstIterator<Key, Value> it)
  {
    Node<Key, Value> *node = const_cast<Node<Key, Value> *>(it.node);
    if (node == fake_leaf || node->parent == nullptr || node->parent->right != node)
    {
      return it;
    }

    Node<Key, Value> *parent = node->parent;
    Node<Key, Value> *grand = parent->parent;

    parent->right = node->left;
    if (node->left != fake_leaf)
    {
      node->left->parent = parent;
    }

    node->left = parent;
    parent->parent = node;

    node->parent = grand;
    if (grand != nullptr)
    {
      if (grand->left == parent)
      {
        grand->left = node;
      }
      else
      {
        grand->right = node;
      }
    }
    else
    {
      root = node;
    }
    return BSTConstIterator<Key, Value>(node, fake_leaf);
  }

  template <class Key, class Value, class Compare>
  BSTConstIterator<Key, Value> BSTree<Key, Value, Compare>::rotateRight(BSTConstIterator<Key, Value> it)
  {
    Node<Key, Value> *node = const_cast<Node<Key, Value> *>(it.node);
    if (node == fake_leaf || node->parent == nullptr || node->parent->left != node)
    {
      return it;
    }

    Node<Key, Value> *parent = node->parent;
    Node<Key, Value> *grand = parent->parent;

    parent->left = node->right;
    if (node->right != fake_leaf)
    {
      node->right->parent = parent;
    }

    node->right = parent;
    parent->parent = node;

    node->parent = grand;
    if (grand != nullptr)
    {
      if (grand->left == parent)
      {
        grand->left = node;
      }
      else
      {
        grand->right = node;
      }
    }
    else
    {
      root = node;
    }
    return BSTConstIterator<Key, Value>(node, fake_leaf);
  }
}

#endif
