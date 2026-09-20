#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace pozdnyakov
{

  namespace detail
  {
    template< typename Key, typename Value >
    struct TreeNode
    {
      Key key;
      Value value;
      TreeNode *parent;
      TreeNode *left;
      TreeNode *right;
      int height;

      TreeNode(const Key &initKey, const Value &initValue, TreeNode *parentNode = nullptr):
        key(initKey),
        value(initValue),
        parent(parentNode),
        left(nullptr),
        right(nullptr),
        height(1)
      {}
    };
  }

}

#include "bstiterators.hpp"

namespace pozdnyakov
{

  template< class Key, class Value, class Compare = std::less< Key >, bool AllowDuplicates = false >
  class BSTree
  {
  public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

  private:
    using Node = detail::TreeNode< Key, Value >;
    Node *root;
    Compare comparator;

    int getNodeHeight(Node *node) const
    {
      return node ? node->height : 0;
    }

    int getBalanceFactor(Node *node) const
    {
      return node ? getNodeHeight(node->right) - getNodeHeight(node->left) : 0;
    }

    void fixHeight(Node *node)
    {
      if (!node) {
        return;
      }
      const int hl = getNodeHeight(node->left);
      const int hr = getNodeHeight(node->right);
      node->height = (hl > hr ? hl : hr) + 1;
    }

    Node *rotateRight(Node *p)
    {
      Node *q = p->left;
      p->left = q->right;
      if (q->right) {
        q->right->parent = p;
      }
      q->right = p;
      q->parent = p->parent;
      p->parent = q;
      fixHeight(p);
      fixHeight(q);
      return q;
    }

    Node *rotateLeft(Node *q)
    {
      Node *p = q->right;
      q->right = p->left;
      if (p->left) {
        p->left->parent = q;
      }
      p->left = q;
      p->parent = q->parent;
      q->parent = p;
      fixHeight(q);
      fixHeight(p);
      return p;
    }

    Node *balanceNode(Node *p)
    {
      fixHeight(p);
      if (getBalanceFactor(p) == 2) {
        if (getBalanceFactor(p->right) < 0) {
          p->right = rotateRight(p->right);
        }
        return rotateLeft(p);
      }
      if (getBalanceFactor(p) == -2) {
        if (getBalanceFactor(p->left) > 0) {
          p->left = rotateLeft(p->left);
        }
        return rotateRight(p);
      }
      return p;
    }

    void balanceUp(Node *curr)
    {
      while (curr) {
        Node *parent = curr->parent;
        Node *newSubRoot = balanceNode(curr);
        if (parent) {
          if (parent->left == curr) {
            parent->left = newSubRoot;
          } else {
            parent->right = newSubRoot;
          }
        } else {
          root = newSubRoot;
        }
        curr = parent;
      }
    }

    void clear(Node *node)
    {
      if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
      }
    }

    Node *copyTree(const Node *node, Node *parentNode = nullptr)
    {
      if (!node) {
        return nullptr;
      }
      Node *newNode = new Node(node->key, node->value, parentNode);
      newNode->height = node->height;
      newNode->left = copyTree(node->left, newNode);
      newNode->right = copyTree(node->right, newNode);
      return newNode;
    }

    void replaceNodeInParent(Node *oldNode, Node *newNode)
    {
      if (oldNode->parent) {
        if (oldNode == oldNode->parent->left) {
          oldNode->parent->left = newNode;
        } else {
          oldNode->parent->right = newNode;
        }
      } else {
        root = newNode;
      }
      if (newNode) {
        newNode->parent = oldNode->parent;
      }
    }

    void removeNode(Node *node)
    {
      Node *balanceStart = nullptr;

      if (node->left && node->right) {
        Node *successor = node->right;
        while (successor->left) {
          successor = successor->left;
        }
        node->key = successor->key;
        node->value = successor->value;

        balanceStart = successor->parent;
        replaceNodeInParent(successor, successor->right);
        delete successor;
      } else if (node->left) {
        balanceStart = node->parent;
        replaceNodeInParent(node, node->left);
        delete node;
      } else if (node->right) {
        balanceStart = node->parent;
        replaceNodeInParent(node, node->right);
        delete node;
      } else {
        balanceStart = node->parent;
        replaceNodeInParent(node, nullptr);
        delete node;
      }

      if (balanceStart) {
        balanceUp(balanceStart);
      }
    }

    Node *findNode(Node *node, const Key &key) const
    {
      while (node) {
        if (comparator(key, node->key)) {
          node = node->left;
        } else if (comparator(node->key, key)) {
          node = node->right;
        } else {
          return node;
        }
      }
      return nullptr;
    }

  public:
    BSTree():
      root(nullptr),
      comparator()
    {}

    BSTree(const BSTree &other):
      root(nullptr),
      comparator(other.comparator)
    {
      root = copyTree(other.root);
    }

    ~BSTree()
    {
      clear(root);
    }

    BSTree &operator=(const BSTree &other)
    {
      if (this != &other) {
        clear(root);
        root = copyTree(other.root);
        comparator = other.comparator;
      }
      return *this;
    }

    void push(const Key &key, const Value &value)
    {
      if (!root) {
        root = new Node(key, value);
        return;
      }
      Node *current = root;
      Node *parent = nullptr;
      while (current) {
        parent = current;
        if (comparator(key, current->key)) {
          current = current->left;
        } else if (comparator(current->key, key)) {
          current = current->right;
        } else {
          if (AllowDuplicates) {
            current = current->right;
          } else {
            current->value = value;
            return;
          }
        }
      }
      Node *newNode = new Node(key, value, parent);
      if (comparator(key, parent->key)) {
        parent->left = newNode;
      } else if (comparator(parent->key, key)) {
        parent->right = newNode;
      } else {
        parent->right = newNode;
      }
      balanceUp(parent);
    }

    Value &get(const Key &key)
    {
      Node *node = findNode(root, key);
      if (!node) {
        throw std::out_of_range("Key not found");
      }
      return node->value;
    }

    const Value &get(const Key &key) const
    {
      Node *node = findNode(root, key);
      if (!node) {
        throw std::out_of_range("Key not found");
      }
      return node->value;
    }

    bool contains(const Key &key) const
    {
      return findNode(root, key) != nullptr;
    }

    void remove(const Key &key)
    {
      if (AllowDuplicates) {
        Node *node;
        while ((node = findNode(root, key)) != nullptr) {
          removeNode(node);
        }
      } else {
        Node *node = findNode(root, key);
        if (node) {
          removeNode(node);
        }
      }
    }

    bool empty() const
    {
      return root == nullptr;
    }

    size_t height() const
    {
      return static_cast< size_t >(getNodeHeight(root));
    }

    iterator begin()
    {
      Node *current = root;
      while (current && current->left) {
        current = current->left;
      }
      return iterator(current);
    }

    iterator end()
    {
      return iterator(nullptr);
    }

    const_iterator begin() const
    {
      return cbegin();
    }

    const_iterator end() const
    {
      return cend();
    }

    const_iterator cbegin() const
    {
      Node *current = root;
      while (current && current->left) {
        current = current->left;
      }
      return const_iterator(current);
    }

    const_iterator cend() const
    {
      return const_iterator(nullptr);
    }
  };

  template< class Key, class Value, class Compare, bool AllowDuplicates >
  BSTree< Key, Value, Compare, AllowDuplicates > intersect(const BSTree< Key, Value, Compare, AllowDuplicates > &tree1,
                                                           const BSTree< Key, Value, Compare, AllowDuplicates > &tree2)
  {
    BSTree< Key, Value, Compare, AllowDuplicates > result;
    for (auto it = tree1.begin(); it != tree1.end(); ++it) {
      const Key &key = (*it).first;
      if (tree2.contains(key)) {
        result.push(key, (*it).second);
      }
    }
    return result;
  }

  template< class Key, class Value, class Compare, bool AllowDuplicates >
  BSTree< Key, Value, Compare, AllowDuplicates > union_(const BSTree< Key, Value, Compare, AllowDuplicates > &tree1,
                                                        const BSTree< Key, Value, Compare, AllowDuplicates > &tree2)
  {
    BSTree< Key, Value, Compare, AllowDuplicates > result;
    for (auto it = tree1.begin(); it != tree1.end(); ++it) {
      result.push((*it).first, (*it).second);
    }
    for (auto it = tree2.begin(); it != tree2.end(); ++it) {
      const Key &key = (*it).first;
      if (!tree1.contains(key)) {
        result.push(key, (*it).second);
      }
    }
    return result;
  }

  template< class Key, class Value, class Compare, bool AllowDuplicates >
  BSTree< Key, Value, Compare, AllowDuplicates > complement(const BSTree< Key, Value, Compare, AllowDuplicates > &tree1,
                                                            const BSTree< Key, Value, Compare, AllowDuplicates > &tree2)
  {
    BSTree< Key, Value, Compare, AllowDuplicates > result;
    for (auto it = tree1.begin(); it != tree1.end(); ++it) {
      const Key &key = (*it).first;
      if (!tree2.contains(key)) {
        result.push(key, (*it).second);
      }
    }
    return result;
  }

}

#endif
