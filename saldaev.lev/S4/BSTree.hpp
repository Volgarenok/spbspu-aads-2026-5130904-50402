#ifndef BSTREE_HPP
#define BSTREE_HPP
#include "../common/vector.hpp"

namespace saldaev
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    struct TreeNode
    {
      std::pair< Key, Value > keyValue_;
      TreeNode *left_;
      TreeNode *right_;
      TreeNode *parent_;

      TreeNode();
      TreeNode(Key key, Value value, TreeNode *parent);
    };

    TreeNode *fRoot_, *fLeaf_;
    size_t size_;
    Compare comp_;

    void clearFrom(TreeNode *) noexcept;
    TreeNode *fallLeft(TreeNode *) const noexcept;

  public:
    class BSTIterator
    {
      friend class BSTree;
      BSTree *tree_;
      TreeNode *current_;

    public:
      explicit BSTIterator(TreeNode *node, BSTree *tree);

      std::pair< Key, Value > &operator*();
      std::pair< Key, Value > *operator->();

      BSTIterator &operator++();
      BSTIterator &operator--();
      BSTIterator operator++(int);
      BSTIterator operator--(int);

      bool operator==(const BSTIterator &other) const;
      bool operator!=(const BSTIterator &other) const;
    };

    class BSTConstIterator
    {
      friend class BSTree;
      const BSTree *tree_;
      TreeNode *current_;

    public:
      explicit BSTConstIterator(TreeNode *node, const BSTree *tree);

      std::pair< Key, Value > &operator*();
      std::pair< Key, Value > *operator->();

      BSTConstIterator &operator++();
      BSTConstIterator &operator--();
      BSTConstIterator operator++(int);
      BSTConstIterator operator--(int);

      bool operator==(const BSTConstIterator &other) const;
      bool operator!=(const BSTConstIterator &other) const;
    };

    BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other) noexcept;
    BSTree &operator=(const BSTree &other);
    BSTree &operator=(BSTree &&other) noexcept;
    ~BSTree();

    void push(const Key &k, const Value &v);
    Value &get(Key k);
    const Value &get(Key k) const;
    void drop(Key k);

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear();

    size_t height(BSTConstIterator it) const;
    size_t height() const;

    BSTConstIterator rotateLeft(BSTConstIterator it);
    BSTConstIterator rotateRight(BSTConstIterator it);
    BSTConstIterator rotateLargeLeft(BSTConstIterator it);
    BSTConstIterator rotateLargeRight(BSTConstIterator it);

    BSTIterator begin();
    BSTIterator end() noexcept;
    BSTConstIterator begin() const;
    BSTConstIterator end() const noexcept;
    BSTConstIterator cbegin() const;
    BSTConstIterator cend() const noexcept;

    void swap(BSTree &other);
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::TreeNode::TreeNode():
    keyValue_({Key(), Value()}),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::TreeNode::TreeNode(Key key, Value value, TreeNode *parent):
    keyValue_({key, value}),
    left_(nullptr),
    right_(nullptr),
    parent_(parent)
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    fRoot_(nullptr),
    fLeaf_(nullptr),
    size_(0),
    comp_(Compare{})
  {
    fRoot_ = new TreeNode();
    try {
      fLeaf_ = new TreeNode();
    } catch (...) {
      delete fRoot_;
      throw;
    }
    fRoot_->left_ = fLeaf_;
    fRoot_->right_ = fLeaf_;
    fLeaf_->parent_ = fRoot_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree &other):
    BSTree()
  {
    for (const std::pair< Key, Value > &v : other) {
      push(v.first, v.second);
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree &&other) noexcept:
    fRoot_(other.fRoot_),
    fLeaf_(other.fLeaf_),
    size_(other.size_),
    comp_(other.comp_)
  {
    other.fRoot_ = nullptr;
    other.fLeaf_ = nullptr;
    other.size_ = 0;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > &BSTree< Key, Value, Compare >::operator=(const BSTree &other)
  {
    if (this != std::addressof(other)) {
      BSTree temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > &BSTree< Key, Value, Compare >::operator=(BSTree &&other) noexcept
  {
    if (this != std::addressof(other)) {
      clear();
      fRoot_ = other.fRoot_;
      fLeaf_ = other.fLeaf_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);
      other.fRoot_ = nullptr;
      other.fLeaf_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    if (fRoot_) {
      clear();
    }
    delete fRoot_;
    delete fLeaf_;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key &k, const Value &v)
  {
    if (fRoot_->left_ == fLeaf_) {
      fRoot_->left_ = new TreeNode(k, v, fRoot_);
      fRoot_->left_->left_ = fLeaf_;
      fRoot_->left_->right_ = fLeaf_;
      ++size_;
      return;
    }

    TreeNode *it = fRoot_->left_;
    while (it != fLeaf_) {
      if (comp_(k, it->keyValue_.first)) {
        if (it->left_ == fLeaf_) {
          it->left_ = new TreeNode(k, v, it);
          it->left_->left_ = fLeaf_;
          it->left_->right_ = fLeaf_;
          ++size_;
          return;
        }
        it = it->left_;

      } else if (comp_(it->keyValue_.first, k)) {
        if (it->right_ == fLeaf_) {
          it->right_ = new TreeNode(k, v, it);
          it->right_->left_ = fLeaf_;
          it->right_->right_ = fLeaf_;
          ++size_;
          return;
        }
        it = it->right_;

      } else {
        it->keyValue_.second = v;
        return;
      }
    }
  }

  template< class Key, class Value, class Compare >
  Value &BSTree< Key, Value, Compare >::get(Key k)
  {
    if (fRoot_->left_ == fLeaf_) {
      throw std::out_of_range("no such key");
    }

    TreeNode *it = fRoot_->left_;
    while (it != fLeaf_) {
      if (comp_(k, it->keyValue_.first)) {
        it = it->left_;
      } else if (comp_(it->keyValue_.first, k)) {
        it = it->right_;
      } else {
        return it->keyValue_.second;
      }
    }
    throw std::out_of_range("no such key");
  }

  template< class Key, class Value, class Compare >
  const Value &BSTree< Key, Value, Compare >::get(Key k) const
  {
    if (fRoot_->left_ == fLeaf_) {
      throw std::out_of_range("no such key");
    }

    TreeNode *it = fRoot_->left_;
    while (it != fLeaf_) {
      if (comp_(k, it->keyValue_.first)) {
        it = it->left_;
      } else if (comp_(it->keyValue_.first, k)) {
        it = it->right_;
      } else {
        return it->keyValue_.second;
      }
    }
    throw std::out_of_range("no such key");
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::drop(Key k)
  {
    if (fRoot_->left_ == fLeaf_) {
      throw std::out_of_range("no such key");
    }

    TreeNode *it = fRoot_->left_;
    while (it != fLeaf_) {
      if (comp_(k, it->keyValue_.first)) {
        it = it->left_;
      } else if (comp_(it->keyValue_.first, k)) {
        it = it->right_;
      } else {
        if (it->left_ == fLeaf_) {
          if (comp_(it->keyValue_.first, it->parent_->left_->keyValue_.first)) {
            it->parent_->left_ = it->right_;
          } else {
            it->parent_->right_ = it->right_;
          }

          if (it->right_ != fLeaf_) {
            it->right_->parent_ = it->parent_;
          }

          delete it;
          --size_;
          return;
        } else if (it->right_ == fLeaf_) {
          if (comp_(it->keyValue_.first, it->parent_->left_->keyValue_.first)) {
            it->parent_->left_ = it->left_;
          } else {
            it->parent_->right_ = it->left_;
          }

          if (it->left_ != fLeaf_) {
            it->left_->parent_ = it->parent_;
          }

          delete it;
          --size_;
          return;
        } else {
          TreeNode *successor = it->right_;
          while (successor->left_ != fLeaf_) {
            successor = successor->left_;
          }

          it->keyValue_.first = successor->keyValue_.first;
          it->keyValue_.second = successor->keyValue_.second;

          if (successor->parent_->left_ == successor) {
            successor->parent_->left_ = successor->right_;
          } else {
            successor->parent_->right_ = successor->right_;
          }
          if (successor->right_ != fLeaf_) {
            successor->right_->parent_ = successor->parent_;
          }
          delete successor;
          --size_;
          return;
        }
      }
    }
    throw std::out_of_range("no such key");
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const noexcept
  {
    return !size_;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
  {
    clearFrom(fRoot_->left_);
    fRoot_->left_ = fLeaf_;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(BSTConstIterator it) const
  {
    if (it.current_ == fLeaf_) {
      return 0;
    }

    size_t levi = height(BSTConstIterator(it.current_->left_, this));
    size_t pravi = height(BSTConstIterator(it.current_->right_, this));
    return std::max(levi, pravi) + 1;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return height(BSTConstIterator(fRoot_->left_, this));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator
  BSTree< Key, Value, Compare >::rotateLeft(BSTConstIterator it)
  {
    TreeNode *parent = it.current_;
    if (parent == fRoot_ || parent->right_ == fLeaf_) {
      return it;
    }
    TreeNode *child = parent->right_;
    parent->right_ = child->left_;
    if (child->left_) {
      child->left_->parent_ = parent;
    }
    child->parent_ = parent->parent_;
    if (!parent->parent_) {
      fRoot_->left_ = child;
    } else if (parent == parent->parent_->left_) {
      parent->parent_->left_ = child;
    } else {
      parent->parent_->right_ = child;
    }
    child->left_ = parent;
    parent->parent_ = child;
    return BSTConstIterator(child, this);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator
  BSTree< Key, Value, Compare >::rotateRight(BSTConstIterator it)
  {
    TreeNode *parrent = it.current_;
    if (parrent == fRoot_ || parrent->left_ == fLeaf_) {
      return it;
    }
    TreeNode *child = parrent->left_;
    parrent->left_ = child->right_;
    if (child->right_) {
      child->right_->parent_ = parrent;
    }
    child->parent_ = parrent->parent_;
    if (!parrent->parent_) {
      fRoot_->left_ = child;
    } else if (parrent == parrent->parent_->left_) {
      parrent->parent_->left_ = child;
    } else {
      parrent->parent_->right_ = child;
    }
    child->right_ = parrent;
    parrent->parent_ = child;
    return BSTConstIterator(child, this);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator
  BSTree< Key, Value, Compare >::rotateLargeLeft(BSTConstIterator it)
  {
    TreeNode *node = it.current_;
    if (node == fRoot_ || node->left_ == fLeaf_ || node->left_->right_ == fLeaf_) {
      return it;
    }
    rotateRight(BSTConstIterator(node->left_, this));
    return rotateLeft(it);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator
  BSTree< Key, Value, Compare >::rotateLargeRight(BSTConstIterator it)
  {
    TreeNode *node = it.current_;
    if (node == fRoot_ || node->right_ == fLeaf_ || node->right_->left_ == fLeaf_) {
      return it;
    }
    rotateLeft(BSTConstIterator(node->right_, this));
    return rotateRight(it);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTIterator BSTree< Key, Value, Compare >::begin()
  {
    if (empty()) {
      return end();
    }
    return BSTIterator(fallLeft(fRoot_->left_), this);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTIterator BSTree< Key, Value, Compare >::end() noexcept
  {
    return BSTIterator(fRoot_, this);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator BSTree< Key, Value, Compare >::begin() const
  {
    if (empty()) {
      return end();
    }
    return BSTConstIterator(fallLeft(fRoot_->left_), this);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator BSTree< Key, Value, Compare >::end() const noexcept
  {
    return BSTConstIterator(fRoot_, this);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator BSTree< Key, Value, Compare >::cbegin() const
  {
    if (empty()) {
      return cend();
    }
    return BSTConstIterator(fallLeft(fRoot_->left_), this);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator BSTree< Key, Value, Compare >::cend() const noexcept
  {
    return BSTConstIterator(fRoot_, this);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree &other)
  {
    std::swap(other.fRoot_, fRoot_);
    std::swap(other.fLeaf_, fLeaf_);
    std::swap(other.size_, size_);
    std::swap(other.comp_, comp_);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clearFrom(TreeNode *node) noexcept
  {
    if (node != fLeaf_) {
      clearFrom(node->left_);
      clearFrom(node->right_);
      delete node;
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTIterator::BSTIterator(TreeNode *node, BSTree *tree):
    tree_(tree),
    current_(node)
  {}

  template< class Key, class Value, class Compare >
  std::pair< Key, Value > &BSTree< Key, Value, Compare >::BSTIterator::operator*()
  {
    return current_->keyValue_;
  }

  template< class Key, class Value, class Compare >
  std::pair< Key, Value > *BSTree< Key, Value, Compare >::BSTIterator::operator->()
  {
    return &(current_->keyValue_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTIterator &BSTree< Key, Value, Compare >::BSTIterator::operator++()
  {
    if (current_ == tree_->fRoot_) {
      return *this;
    }

    if (current_->right_ != tree_->fLeaf_) {
      current_ = current_->right_;
      while (current_->left_ != tree_->fLeaf_) {
        current_ = current_->left_;
      }
    } else {
      TreeNode *parent = current_->parent_;
      while (parent != tree_->fRoot_ && current_ == parent->right_) {
        current_ = parent;
        parent = current_->parent_;
      }
      current_ = parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTIterator &BSTree< Key, Value, Compare >::BSTIterator::operator--()
  {
    if (current_ == tree_->fRoot_) {
      return *this;
    }

    if (current_->left_ != tree_->fLeaf_) {
      current_ = current_->left_;
      while (current_->right_ != fLeaf_) {
        current_ = current_->right_;
      }
    } else {
      TreeNode *parent = current_->parent_;
      while (parent != tree_->fRoot_ && current_ == parent->left_) {
        current_ = parent;
        parent = current_->parent_;
      }
      current_ = parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTIterator BSTree< Key, Value, Compare >::BSTIterator::operator++(int)
  {
    BSTIterator ret = *this;
    ++(*this);
    return ret;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTIterator BSTree< Key, Value, Compare >::BSTIterator::operator--(int)
  {
    BSTIterator ret = *this;
    --(*this);
    return ret;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::BSTIterator::operator==(const BSTIterator &other) const
  {
    return current_ == other.current_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::BSTIterator::operator!=(const BSTIterator &other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTConstIterator::BSTConstIterator(TreeNode *node, const BSTree *tree):
    tree_(tree),
    current_(node)
  {}

  template< class Key, class Value, class Compare >
  std::pair< Key, Value > &BSTree< Key, Value, Compare >::BSTConstIterator::operator*()
  {
    return current_->keyValue_;
  }

  template< class Key, class Value, class Compare >
  std::pair< Key, Value > *BSTree< Key, Value, Compare >::BSTConstIterator::operator->()
  {
    return &(current_->keyValue_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator &
  BSTree< Key, Value, Compare >::BSTConstIterator::operator++()
  {
    if (current_ == tree_->fRoot_) {
      return *this;
    }

    if (current_->right_ != tree_->fLeaf_) {
      current_ = current_->right_;
      while (current_->left_ != tree_->fLeaf_) {
        current_ = current_->left_;
      }
    } else {
      TreeNode *parent = current_->parent_;
      while (parent != tree_->fRoot_ && current_ == parent->right_) {
        current_ = parent;
        parent = current_->parent_;
      }
      current_ = parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator &
  BSTree< Key, Value, Compare >::BSTConstIterator::operator--()
  {
    if (current_ == tree_->fRoot_) {
      return *this;
    }

    if (current_->left_ != tree_->fLeaf_) {
      current_ = current_->left_;
      while (current_->right_ != fLeaf_) {
        current_ = current_->right_;
      }
    } else {
      TreeNode *parent = current_->parent_;
      while (parent != tree_->fRoot_ && current_ == parent->left_) {
        current_ = parent;
        parent = current_->parent_;
      }
      current_ = parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator
  BSTree< Key, Value, Compare >::BSTConstIterator::operator++(int)
  {
    BSTConstIterator ret = *this;
    ++(*this);
    return ret;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::BSTConstIterator
  BSTree< Key, Value, Compare >::BSTConstIterator::operator--(int)
  {
    BSTConstIterator ret = *this;
    --(*this);
    return ret;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::BSTConstIterator::operator==(const BSTConstIterator &other) const
  {
    return current_ == other.current_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::BSTConstIterator::operator!=(const BSTConstIterator &other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::TreeNode *
  BSTree< Key, Value, Compare >::fallLeft(TreeNode *node) const noexcept
  {
    while (node->left_ != fLeaf_) {
      node = node->left_;
    }
    return node;
  }
}

#endif
