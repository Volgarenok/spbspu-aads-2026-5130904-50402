#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

namespace samarin {

  namespace detail {
    template< class Key, class Value >
    struct TreeNode {
      std::pair< Key, Value > data;
      TreeNode< Key, Value > * left;
      TreeNode< Key, Value > * right;
      TreeNode< Key, Value > * parent;
    };

    template< class Node >
    Node * leftmost(Node * node)
    {
      while (node != nullptr && node->left != nullptr) {
        node = node->left;
      }
      return node;
    }

    template< class Node >
    Node * successor(Node * node)
    {
      if (node->right != nullptr) {
        return leftmost(node->right);
      }
      Node * parent = node->parent;
      while (parent != nullptr && node == parent->right) {
        node = parent;
        parent = parent->parent;
      }
      return parent;
    }
  }

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  class BSTIterator {
  public:
    std::pair< Key, Value > & operator*() const
    {
      return node_->data;
    }

    std::pair< Key, Value > * operator->() const
    {
      return std::addressof(node_->data);
    }

    BSTIterator< Key, Value > & operator++()
    {
      node_ = detail::successor(node_);
      return *this;
    }

    BSTIterator< Key, Value > operator++(int)
    {
      BSTIterator< Key, Value > tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTIterator< Key, Value > & other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const BSTIterator< Key, Value > & other) const
    {
      return node_ != other.node_;
    }

  private:
    template< class K, class V, class C >
    friend class BSTree;
    friend class BSTConstIterator< Key, Value >;
    detail::TreeNode< Key, Value > * node_;

    explicit BSTIterator(detail::TreeNode< Key, Value > * node):
      node_(node)
    {}
  };

  template< class Key, class Value >
  class BSTConstIterator {
  public:
    BSTConstIterator(const BSTIterator< Key, Value > & other):
      node_(other.node_)
    {}

    const std::pair< Key, Value > & operator*() const
    {
      return node_->data;
    }

    const std::pair< Key, Value > * operator->() const
    {
      return std::addressof(node_->data);
    }

    BSTConstIterator< Key, Value > & operator++()
    {
      node_ = detail::successor(node_);
      return *this;
    }

    BSTConstIterator< Key, Value > operator++(int)
    {
      BSTConstIterator< Key, Value > tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTConstIterator< Key, Value > & other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const BSTConstIterator< Key, Value > & other) const
    {
      return node_ != other.node_;
    }

  private:
    template< class K, class V, class C >
    friend class BSTree;
    const detail::TreeNode< Key, Value > * node_;

    explicit BSTConstIterator(const detail::TreeNode< Key, Value > * node):
      node_(node)
    {}
  };

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree {
  public:
    using value_type = std::pair< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree():
      root_(nullptr),
      size_(0),
      cmp_()
    {}

    BSTree(const BSTree< Key, Value, Compare > & other):
      root_(clone(other.root_, nullptr)),
      size_(other.size_),
      cmp_(other.cmp_)
    {}

    BSTree(BSTree< Key, Value, Compare > && other) noexcept:
      root_(other.root_),
      size_(other.size_),
      cmp_(std::move(other.cmp_))
    {
      other.root_ = nullptr;
      other.size_ = 0;
    }

    ~BSTree()
    {
      destroy(root_);
    }

    BSTree< Key, Value, Compare > & operator=(const BSTree< Key, Value, Compare > & other)
    {
      if (this != std::addressof(other)) {
        BSTree< Key, Value, Compare > tmp(other);
        swap(tmp);
      }
      return *this;
    }

    BSTree< Key, Value, Compare > & operator=(BSTree< Key, Value, Compare > && other) noexcept
    {
      if (this != std::addressof(other)) {
        clear();
        root_ = other.root_;
        size_ = other.size_;
        cmp_ = std::move(other.cmp_);
        other.root_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    void push(const Key & key, const Value & value)
    {
      detail::TreeNode< Key, Value > * parent = nullptr;
      detail::TreeNode< Key, Value > * cur = root_;
      bool toLeft = false;
      while (cur != nullptr) {
        parent = cur;
        if (cmp_(key, cur->data.first)) {
          cur = cur->left;
          toLeft = true;
        } else if (cmp_(cur->data.first, key)) {
          cur = cur->right;
          toLeft = false;
        } else {
          Value replacement(value);
          cur->data.second = std::move(replacement);
          return;
        }
      }
      detail::TreeNode< Key, Value > * node = makeNode(key, value, parent);
      if (parent == nullptr) {
        root_ = node;
      } else if (toLeft) {
        parent->left = node;
      } else {
        parent->right = node;
      }
      ++size_;
    }

    iterator find(const Key & key)
    {
      return iterator(findNode(key));
    }

    const_iterator find(const Key & key) const
    {
      return const_iterator(findNode(key));
    }

    bool contains(const Key & key) const
    {
      return findNode(key) != nullptr;
    }

    Value get(const Key & key) const
    {
      const detail::TreeNode< Key, Value > * node = findNode(key);
      if (node == nullptr) {
        throw std::out_of_range("no such key");
      }
      return node->data.second;
    }

    bool empty() const
    {
      return size_ == 0;
    }

    std::size_t size() const
    {
      return size_;
    }

    std::size_t height() const
    {
      return heightOf(root_);
    }

    std::size_t height(const_iterator it) const
    {
      return heightOf(it.node_);
    }

    const_iterator rotateRight(const_iterator it)
    {
      detail::TreeNode< Key, Value > * pivot = nodeOf(it);
      if (pivot == nullptr || pivot->parent == nullptr || pivot->parent->left != pivot) {
        throw std::logic_error("invalid right rotation");
      }
      detail::TreeNode< Key, Value > * parent = pivot->parent;
      detail::TreeNode< Key, Value > * middle = pivot->right;
      transplant(parent, pivot);
      pivot->right = parent;
      parent->parent = pivot;
      parent->left = middle;
      if (middle != nullptr) {
        middle->parent = parent;
      }
      return const_iterator(pivot);
    }

    const_iterator rotateLeft(const_iterator it)
    {
      detail::TreeNode< Key, Value > * pivot = nodeOf(it);
      if (pivot == nullptr || pivot->parent == nullptr || pivot->parent->right != pivot) {
        throw std::logic_error("invalid left rotation");
      }
      detail::TreeNode< Key, Value > * parent = pivot->parent;
      detail::TreeNode< Key, Value > * middle = pivot->left;
      transplant(parent, pivot);
      pivot->left = parent;
      parent->parent = pivot;
      parent->right = middle;
      if (middle != nullptr) {
        middle->parent = parent;
      }
      return const_iterator(pivot);
    }

    const_iterator rotateLargeRight(const_iterator it)
    {
      rotateLeft(it);
      return rotateRight(it);
    }

    const_iterator rotateLargeLeft(const_iterator it)
    {
      rotateRight(it);
      return rotateLeft(it);
    }

    Value drop(const Key & key)
    {
      detail::TreeNode< Key, Value > * victim = findNode(key);
      if (victim == nullptr) {
        throw std::out_of_range("no such key");
      }
      if (victim->left == nullptr) {
        transplant(victim, victim->right);
      } else if (victim->right == nullptr) {
        transplant(victim, victim->left);
      } else {
        detail::TreeNode< Key, Value > * heir = detail::leftmost(victim->right);
        if (heir->parent != victim) {
          transplant(heir, heir->right);
          heir->right = victim->right;
          heir->right->parent = heir;
        }
        transplant(victim, heir);
        heir->left = victim->left;
        heir->left->parent = heir;
      }
      Value value = std::move(victim->data.second);
      delete victim;
      --size_;
      return value;
    }

    void clear()
    {
      destroy(root_);
      root_ = nullptr;
      size_ = 0;
    }

    void swap(BSTree< Key, Value, Compare > & other) noexcept
    {
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
      std::swap(cmp_, other.cmp_);
    }

    iterator begin()
    {
      return iterator(detail::leftmost(root_));
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
      return const_iterator(detail::leftmost(root_));
    }

    const_iterator cend() const
    {
      return const_iterator(nullptr);
    }

  private:
    detail::TreeNode< Key, Value > * root_;
    std::size_t size_;
    Compare cmp_;

    detail::TreeNode< Key, Value > * findNode(const Key & key) const
    {
      detail::TreeNode< Key, Value > * cur = root_;
      while (cur != nullptr) {
        if (cmp_(key, cur->data.first)) {
          cur = cur->left;
        } else if (cmp_(cur->data.first, key)) {
          cur = cur->right;
        } else {
          return cur;
        }
      }
      return nullptr;
    }

    static detail::TreeNode< Key, Value > * nodeOf(const_iterator it)
    {
      return const_cast< detail::TreeNode< Key, Value > * >(it.node_);
    }

    void transplant(detail::TreeNode< Key, Value > * target,
        detail::TreeNode< Key, Value > * replacement)
    {
      if (target->parent == nullptr) {
        root_ = replacement;
      } else if (target == target->parent->left) {
        target->parent->left = replacement;
      } else {
        target->parent->right = replacement;
      }
      if (replacement != nullptr) {
        replacement->parent = target->parent;
      }
    }

    static detail::TreeNode< Key, Value > * makeNode(const Key & key, const Value & value,
        detail::TreeNode< Key, Value > * parent)
    {
      return new detail::TreeNode< Key, Value >{ value_type(key, value), nullptr, nullptr, parent };
    }

    static detail::TreeNode< Key, Value > * clone(const detail::TreeNode< Key, Value > * node,
        detail::TreeNode< Key, Value > * parent)
    {
      if (node == nullptr) {
        return nullptr;
      }
      detail::TreeNode< Key, Value > * fresh =
          new detail::TreeNode< Key, Value >{ node->data, nullptr, nullptr, parent };
      try {
        fresh->left = clone(node->left, fresh);
        fresh->right = clone(node->right, fresh);
      } catch (...) {
        destroy(fresh);
        throw;
      }
      return fresh;
    }

    static void destroy(detail::TreeNode< Key, Value > * node)
    {
      if (node == nullptr) {
        return;
      }
      destroy(node->left);
      destroy(node->right);
      delete node;
    }

    static std::size_t heightOf(const detail::TreeNode< Key, Value > * node)
    {
      if (node == nullptr) {
        return 0;
      }
      const std::size_t left = heightOf(node->left);
      const std::size_t right = heightOf(node->right);
      return 1 + (left > right ? left : right);
    }
  };

}

#endif
