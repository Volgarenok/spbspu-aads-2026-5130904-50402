#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <utility>
#include <functional>
#include <cstddef>
#include <stdexcept>

namespace muhamadiarov
{
  template< class Key, class Value >
  struct TreeNode
  {
    std::pair< Key, Value > val_;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode* parent_;

    TreeNode();
    TreeNode(const Key& key, const Value& value, TreeNode* parent);
    TreeNode(Key&& key, Value&& value, TreeNode* parent);
  };
  template< class Key, class Value >
  class BSIterator;
  template< class Key, class Value >
  class BSConstIterator;
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using Iter_t = BSIterator< Key, Value >;
    using ConstIter_t = BSConstIterator< Key, Value >;
    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree();

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void push(const Key& k, const Value& v);
    Value& get(Key k);
    const Value& get(Key k) const;
    void drop(Key k);
    bool has(const Key& k) const noexcept;

    ConstIter_t rotateLeft(ConstIter_t it);
    ConstIter_t rotateRight(ConstIter_t it);

    ConstIter_t rotateLargeLeft(ConstIter_t it);
    ConstIter_t rotateLargeRight(ConstIter_t it);

    size_t height(ConstIter_t it) const;
    size_t height() const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void swap(BSTree& other) noexcept;

    void clear();

    Iter_t begin();
    Iter_t end() noexcept;
    ConstIter_t begin() const;
    ConstIter_t end() const noexcept;
    ConstIter_t cbegin() const;
    ConstIter_t cend() const noexcept;

  private:
    TreeNode< Key, Value >* root_;
    size_t size_;
    Compare comp_;

    void copyTree(TreeNode< Key, Value >* source, TreeNode< Key, Value >* parent);
    TreeNode< Key, Value >* findNode(const Key& k) const noexcept;
    void clearSubtree(TreeNode< Key, Value >* node) noexcept;
    TreeNode< Key, Value >* getMinimum(TreeNode< Key, Value >* node) const noexcept;
    TreeNode< Key, Value >* getMaximum(TreeNode< Key, Value >* node) const noexcept;
    size_t getHeight(const TreeNode< Key, Value >* node) const noexcept;
  };

  template< class Key, class Value >
  class BSIterator
  {
  public:
    BSIterator();
    explicit BSIterator(TreeNode< Key, Value >* node);
    BSIterator(const BSIterator&) = default;
    BSIterator& operator=(const BSIterator&) = default;

    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();

    BSIterator& operator++();
    BSIterator operator++(int);

    BSIterator& operator--();
    BSIterator operator--(int);

    bool operator==(const BSIterator& other) const;
    bool operator!=(const BSIterator& other) const;
  private:
    TreeNode< Key, Value >* curr_;
    template< class K, class V, class C > friend class BSTree;

    static TreeNode< Key, Value >* getNext(TreeNode< Key, Value >* node);
    static TreeNode< Key, Value >* getPrevious(TreeNode< Key, Value >* node);
  };

  template< class Key, class Value >
  class BSConstIterator
  {
  public:
    BSConstIterator();
    explicit BSConstIterator(TreeNode< Key, Value >* node);
    BSConstIterator(const BSConstIterator&) = default;
    BSConstIterator& operator=(const BSConstIterator&) = default;

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    BSConstIterator& operator++();
    BSConstIterator operator++(int);

    BSConstIterator& operator--();
    BSConstIterator operator--(int);

    bool operator==(const BSConstIterator& other) const;
    bool operator!=(const BSConstIterator& other) const;
  private:
    TreeNode< Key, Value >* curr_;
    template< class K, class V, class C > friend class BSTree;

    static TreeNode< Key, Value >* getNext(TreeNode< Key, Value >* node);
    static TreeNode< Key, Value >* getPrevious(TreeNode< Key, Value >* node);
  };
}

namespace muh = muhamadiarov;

template< class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode():
  val_({Key(), Value()}),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}

template< class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode(const Key& key, const Value& value, TreeNode* parent):
  val_({key, value}),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template< class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode(Key&& key, Value&& value, TreeNode* parent):
  val_({std::move(key), std::move(value)}),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::BSTree():
  root_(new TreeNode< Key, Value >()),
  size_(0),
  comp_()
{}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  root_(new TreeNode< Key, Value >()),
  size_(0),
  comp_(other.comp_)
{
  if (!other.empty())
  {
    copyTree(other.root_->left_, nullptr);
  }
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  root_(other.root_),
  size_(other.size_),
  comp_(std::move(other.comp_))
{
  other.root_ = nullptr;
  other.size_ = 0;
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::~BSTree()
{
  if (root_)
  {
    clear();
    delete root_;
  }
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >& muh::BSTree< Key, Value, Compare >::operator=(
  const BSTree& other
)
{
  if (this != &other)
  {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >& muh::BSTree< Key, Value, Compare >::operator=(
  BSTree&& other
) noexcept
{
  if (this != std::addressof(other))
  {
    clear();
    delete root_;
    root_ = other.root_;
    size_ = other.size_;
    comp_ = std::move(other.comp_);
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template< class Key, class Value, class Compare >
void muh::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template< class Key, class Value, class Compare >
void muh::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  TreeNode<Key, Value>* parent = nullptr;
  TreeNode<Key, Value>* current = root_->left_;

  while (current)
  {
    parent = current;
    if (comp_(k, current->val_.first))
    {
      current = current->left_;
    }
    else if (comp_(current->val_.first, k))
    {
      current = current->right_;
    }
    else
    {
      current->val_.second = v;
      return;
    }
  }

  TreeNode<Key, Value>* newNode = new TreeNode<Key, Value>(k, v, parent);

  if (!parent)
  {
    root_->left_ = newNode;
  }
  else if (comp_(k, parent->val_.first))
  {
    parent->left_ = newNode;
  }
  else
  {
    parent->right_ = newNode;
  }
  ++size_;
}

template< class Key, class Value, class Compare >
Value& muh::BSTree< Key, Value, Compare >::get(Key k)
{
  TreeNode<Key, Value>* node = findNode(k);
  if (!node)
  {
    throw std::out_of_range("Key not found");
  }
  return node->val_.second;
}

template< class Key, class Value, class Compare >
const Value& muh::BSTree< Key, Value, Compare >::get(Key k) const
{
  const TreeNode<Key, Value>* node = findNode(k);
  if (!node)
  {
    throw std::out_of_range("Key not found");
  }
  return node->val_.second;
}

template< class Key, class Value, class Compare >
void muh::BSTree< Key, Value, Compare >::drop(Key k)
{
  TreeNode<Key, Value>* node = findNode(k);
  if (!node)
  {
    throw std::out_of_range("Key not found");
  }

  if (node->left_ && node->right_)
  {
    TreeNode<Key, Value>* succ = getMinimum(node->right_);
    node->val_.first = std::move(succ->val_.first);
    node->val_.second = std::move(succ->val_.second);
    node = succ;
  }

  TreeNode<Key, Value>* child = (node->left_) ? node->left_ : node->right_;

  if (child)
  {
    child->parent_ = node->parent_;
  }

  if (!node->parent_)
  {
    root_->left_ = child;
  }
  else if (node->parent_->left_ == node)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
  }

  delete node;
  --size_;
}

template<class Key, class Value, class Compare>
muh::TreeNode<Key, Value>* muh::BSTree<Key, Value, Compare>::findNode(
  const Key& k
) const noexcept
{
  TreeNode<Key, Value>* current = root_->left_;
  while (current)
  {
    if (comp_(k, current->val_.first))
    {
      current = current->left_;
    }
    else if (comp_(current->val_.first, k))
    {
      current = current->right_;
    }
    else {
      return current;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
bool muh::BSTree< Key, Value, Compare >::has(const Key& k) const noexcept
{
  return findNode(k) != nullptr;
}

template< class Key, class Value, class Compare >
size_t muh::BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
bool muh::BSTree< Key, Value, Compare >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Compare >
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::rotateLeft(
  ConstIter_t it
)
{
  TreeNode< Key, Value > *node = it.curr_;
  if (!node || !node->right_)
  {
    return it;
  }

  TreeNode< Key, Value > *child = node->right_;
  node->right_ = child->left_;
  if (child->left_)
  {
    child->left_->parent_ = node;
  }
  child->parent_ = node->parent_;
  if (!node->parent_)
  {
    root_->left_ = child;
  }
  else if (node == node->parent_->left_)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
  }

  child->left_ = node;
  node->parent_ = child;
  return ConstIter_t(child);
}

template< class Key, class Value, class Compare >
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::rotateRight(
  ConstIter_t it
)
{
  TreeNode< Key, Value > *node = it.curr_;
  if (!node || !node->left_)
  {
    return it;
  }

  TreeNode< Key, Value > *child = node->left_;
  node->left_ = child->right_;
  if (child->right_)
  {
    child->right_->parent_ = node;
  }
  child->parent_ = node->parent_;
  if (!node->parent_)
  {
    root_->left_ = child;
  }
  else if (node == node->parent_->left_)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
  }

  child->right_ = node;
  node->parent_ = child;
  return  ConstIter_t(child);
}

template< class Key, class Value, class Compare >
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::rotateLargeLeft(
  ConstIter_t it
)
{
  TreeNode< Key, Value > *node = it.curr_;
  if (!node || !node->right_ || !node->right_->left_)
  {
    return it;
  }
  rotateRight(ConstIter_t(node->right_));
  return rotateLeft(it);
}

template< class Key, class Value, class Compare >
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::rotateLargeRight(
  ConstIter_t it
)
{
  TreeNode< Key, Value > *node = it.curr_;
  if (!node || !node->left_ || !node->left_->right_)
  {
    return it;
  }
  rotateLeft(ConstIter_t(node->left_));
  return rotateRight(it);
}

template< class Key, class Value, class Compare >
size_t muh::BSTree< Key, Value, Compare >::height(ConstIter_t it) const
{
  return getHeight(it.curr_);
}

template< class Key, class Value, class Compare >
size_t muh::BSTree< Key, Value, Compare >::height() const
{
  return getHeight(root_->left_);
}

template<class Key, class Value, class Compare>
size_t muh::BSTree<Key, Value, Compare>::getHeight(
  const TreeNode<Key, Value>* node
) const noexcept
{
  if (!node)
  {
    return 0;
  }
  return 1 + std::max(getHeight(node->left_), getHeight(node->right_));
}

template<class Key, class Value, class Compare>
void muh::BSTree<Key, Value, Compare>::copyTree(
  TreeNode< Key, Value >* source,
  TreeNode< Key, Value >* parent
)
{
  if (!source)
  {
    return;
  }

  TreeNode<Key, Value>* newNode = new TreeNode<Key, Value>(
    source->val_.first,
    source->val_.second,
    parent
  );

  if (!parent)
  {
    root_->left_ = newNode;
  }
  else if (comp_(source->val_.first, parent->val_.first))
  {
    parent->left_ = newNode;
  }
  else
  {
    parent->right_ = newNode;
  }

  copyTree(source->left_, newNode);
  copyTree(source->right_, newNode);
  ++size_;
}

template< class Key, class Value, class Compare >
void muh::BSTree< Key, Value, Compare >::clear()
{
  if (root_)
  {
    clearSubtree(root_->left_);
    root_->left_ = nullptr;
  }
  size_ = 0;
}

template< class Key, class Value, class Compare >
void muh::BSTree< Key, Value, Compare >::clearSubtree(TreeNode< Key, Value >* node) noexcept
{
  if (!node)
  {
    return;
  }
  clearSubtree(node->left_);
  clearSubtree(node->right_);
  delete node;
}

template<class Key, class Value, class Compare>
muh::TreeNode<Key, Value>* muh::BSTree<Key, Value, Compare>::getMinimum(
  TreeNode<Key, Value>* node
) const noexcept
{
  if (!node)
  {
    return nullptr;
  }
  while (node->left_)
  {
    node = node->left_;
  }
  return node;
}

template<class Key, class Value, class Compare>
muh::TreeNode<Key, Value>* muh::BSTree<Key, Value, Compare>::getMaximum(
  TreeNode<Key, Value>* node
) const noexcept
{
  if (!node)
  {
    return nullptr;
  }
  while (node->right_)
  {
    node = node->right_;
  }
  return node;
}

template<class Key, class Value, class Compare>
muh::BSIterator< Key, Value > muh::BSTree< Key, Value, Compare >::begin()
{
  if (!root_)
  {
    return Iter_t(nullptr);
  }
  return Iter_t(getMinimum(root_->left_));
}

template<class Key, class Value, class Compare>
muh::BSIterator< Key, Value > muh::BSTree< Key, Value, Compare >::end() noexcept
{
  return Iter_t(nullptr);
}

template<class Key, class Value, class Compare>
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::begin() const
{
  if (!root_)
  {
    return ConstIter_t(nullptr);
  }
  return ConstIter_t(getMinimum(root_->left_));
}

template<class Key, class Value, class Compare>
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::end() const noexcept
{
    return ConstIter_t(nullptr);
}

template<class Key, class Value, class Compare>
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::cbegin() const
{
    return begin();
}

template<class Key, class Value, class Compare>
muh::BSConstIterator< Key, Value > muh::BSTree< Key, Value, Compare >::cend() const noexcept
{
    return end();
}

template< class Key, class Value >
muh::BSIterator< Key, Value >::BSIterator():
  curr_(nullptr)
{}

template< class Key, class Value >
muh::BSIterator< Key, Value >::BSIterator(TreeNode< Key, Value >* node):
  curr_(node)
{}

template< class Key, class Value >
std::pair< Key, Value >& muh::BSIterator< Key, Value >::operator*()
{
  if (!curr_)
  {
    throw std::runtime_error("Dereferencing nullptr");
  }
  return curr_->val_;
}

template< class Key, class Value >
std::pair< Key, Value >* muh::BSIterator< Key, Value >::operator->()
{
  if (!curr_)
  {
    throw std::runtime_error("Dereferencing nullptr");
  }
  return &(curr_->val_);
}

template< class Key, class Value >
muh::BSIterator< Key, Value >& muh::BSIterator< Key, Value >::operator++()
{
  curr_ = getNext(curr_);
  return *this;
}

template< class Key, class Value >
muh::BSIterator< Key, Value > muh::BSIterator< Key, Value >::operator++(int)
{
  BSIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
muh::BSIterator< Key, Value >& muh::BSIterator< Key, Value >::operator--()
{
  curr_ = getPrevious(curr_);
  return *this;
}

template< class Key, class Value >
muh::BSIterator< Key, Value > muh::BSIterator< Key, Value >::operator--(int)
{
  BSIterator temp(*this);
  --(*this);
  return temp;
}

template< class Key, class Value >
bool muh::BSIterator< Key, Value >::operator==(const BSIterator< Key, Value >& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool muh::BSIterator< Key, Value >::operator!=(const BSIterator< Key, Value >& other) const
{
  return !(*this == other);
}

template < class Key, class Value >
muh::TreeNode< Key, Value >* muh::BSIterator< Key, Value >::getNext(
  TreeNode< Key, Value >* node
)
{
  if (!node)
  {
    return nullptr;
  }

  if (node->right_)
  {
    node = node->right_;
    while (node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  TreeNode< Key, Value >* parent = node->parent_;
  while (parent && node == parent->right_)
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}

template< class Key, class Value >
muh::TreeNode< Key, Value >* muh::BSIterator< Key, Value >::getPrevious(
  TreeNode< Key, Value >* node
)
{
  if (!node)
  {
    return nullptr;
  }

  if (node->left_)
  {
    node = node->left_;
    while (node->right_)
    {
      node = node->right_;
    }
    return node;
  }

  TreeNode< Key, Value >* parent = node->parent_;
  while (parent && node == parent->left_)
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}

template< class Key, class Value >
muh::BSConstIterator< Key, Value >::BSConstIterator():
  curr_(nullptr)
{}

template< class Key, class Value >
muh::BSConstIterator< Key, Value >::BSConstIterator(TreeNode< Key, Value >* node):
  curr_(node)
{}

template< class Key, class Value >
const std::pair< Key, Value >& muh::BSConstIterator< Key, Value >::operator*() const
{
  if (!curr_)
  {
    throw std::runtime_error("Dereferencing nullptr");
  }
  return curr_->val_;
}

template< class Key, class Value >
const std::pair< Key, Value >* muh::BSConstIterator< Key, Value >::operator->() const
{
  if (!curr_)
  {
    throw std::runtime_error("Dereferencing nullptr");
  }
  return &(curr_->val_);
}

template< class Key, class Value >
muh::BSConstIterator< Key, Value >& muh::BSConstIterator< Key, Value >::operator++()
{
  curr_ = getNext(curr_);
  return *this;
}

template< class Key, class Value >
muh::BSConstIterator< Key, Value > muh::BSConstIterator< Key, Value >::operator++(int)
{
  BSConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
muh::BSConstIterator< Key, Value >& muh::BSConstIterator< Key, Value >::operator--()
{
  curr_ = getPrevious(curr_);
  return *this;
}

template< class Key, class Value >
muh::BSConstIterator< Key, Value > muh::BSConstIterator< Key, Value >::operator--(int)
{
  BSConstIterator temp(*this);
  --(*this);
  return temp;
}

template< class Key, class Value >
bool muh::BSConstIterator< Key, Value >::operator==(
  const BSConstIterator< Key, Value >& other
) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool muh::BSConstIterator< Key, Value >::operator!=(
  const BSConstIterator< Key, Value >& other
) const
{
  return !(*this == other);
}

template < class Key, class Value >
muh::TreeNode< Key, Value >* muh::BSConstIterator< Key, Value >::getNext(
  TreeNode< Key, Value >* node
)
{
  if (!node)
  {
    return nullptr;
  }

  if (node->right_)
  {
    node = node->right_;
    while (node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  TreeNode< Key, Value >* parent = node->parent_;
  while (parent && node == parent->right_)
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}

template< class Key, class Value >
muh::TreeNode< Key, Value >* muh::BSConstIterator< Key, Value >::getPrevious(
  TreeNode< Key, Value >* node
)
{
  if (!node)
  {
    return nullptr;
  }

  if (node->left_)
  {
    node = node->left_;
    while (node->right_)
    {
      node = node->right_;
    }
    return node;
  }

  TreeNode< Key, Value >* parent = node->parent_;
  while (parent && node == parent->left_)
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}
#endif
