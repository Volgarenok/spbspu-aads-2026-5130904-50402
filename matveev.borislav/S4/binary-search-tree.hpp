#ifndef MATVEEV_BINARY_SEARCH_TREE_HPP
#define MATVEEV_BINARY_SEARCH_TREE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace matveev
{
template< class Key, class Value, class Compare = std::less< Key > >
class BSTree
{
private:
  struct Node
  {
    Node();
    Node(const Key& key, const Value& value);
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    std::pair< Key, Value > data;
    Node* parent;
    Node* left;
    Node* right;
  };

public:
  class ConstIterator;

  class Iterator
  {
    friend class BSTree< Key, Value, Compare >;
    friend class ConstIterator;

  public:
    Iterator();
    Iterator(const Iterator&) = default;
    Iterator& operator=(const Iterator&) = default;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);

    bool operator==(const Iterator& rhs) const noexcept;
    bool operator!=(const Iterator& rhs) const noexcept;

  private:
    Iterator(Node* node, Node* fake);

    Node* node_;
    Node* fake_;
  };

  class ConstIterator
  {
    friend class BSTree< Key, Value, Compare >;

  public:
    ConstIterator();
    ConstIterator(const ConstIterator&) = default;
    ConstIterator& operator=(const ConstIterator&) = default;
    ConstIterator(const Iterator& iter);

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    ConstIterator& operator++();
    ConstIterator operator++(int);
    ConstIterator& operator--();
    ConstIterator operator--(int);

    bool operator==(const ConstIterator& rhs) const noexcept;
    bool operator!=(const ConstIterator& rhs) const noexcept;

  private:
    ConstIterator(Node* node, Node* fake);

    Node* node_;
    Node* fake_;
  };

  BSTree();
  BSTree(const BSTree& other);
  BSTree(BSTree&& other) noexcept;
  ~BSTree();

  BSTree& operator=(const BSTree& other);
  BSTree& operator=(BSTree&& other) noexcept;

  void swap(BSTree& other) noexcept;

  bool empty() const noexcept;
  size_t size() const noexcept;

  void push(const Key& key, const Value& value);
  Value drop(const Key& key);
  bool has(const Key& key) const;

  Value& at(const Key& key);
  const Value& at(const Key& key) const;

  Iterator begin() noexcept;
  Iterator end() noexcept;
  ConstIterator begin() const noexcept;
  ConstIterator end() const noexcept;
  ConstIterator cbegin() const noexcept;
  ConstIterator cend() const noexcept;

  Iterator find(const Key& key) noexcept;
  ConstIterator find(const Key& key) const noexcept;

  size_t height() const noexcept;
  size_t height(ConstIterator pos) const noexcept;

  Iterator rotateLeft(Iterator rising);
  Iterator rotateRight(Iterator rising);

  void clear() noexcept;

private:
  Node* findNode(const Key& key) const noexcept;
  void transplant(Node* old_node, Node* new_node) noexcept;

  static Node* minNode(Node* node, Node* fake) noexcept;
  static Node* maxNode(Node* node, Node* fake) noexcept;
  static size_t heightNode(Node* node) noexcept;
  static void deleteSubtree(Node* node) noexcept;
  static Node* copySubtree(Node* node, Node* parent);

  Node fake_;
  size_t size_;
  Compare cmp_;
};

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::Node::Node():
  data(),
  parent(nullptr),
  left(nullptr),
  right(nullptr)
{}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::Node::Node(const Key& key, const Value& value):
  data(key, value),
  parent(nullptr),
  left(nullptr),
  right(nullptr)
{}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::Iterator::Iterator():
  node_(nullptr),
  fake_(nullptr)
{}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::Iterator::Iterator(Node* node, Node* fake):
  node_(node),
  fake_(fake)
{}

template< class Key, class Value, class Compare >
std::pair< Key, Value >& BSTree< Key, Value, Compare >::Iterator::operator*() const
{
  return node_->data;
}

template< class Key, class Value, class Compare >
std::pair< Key, Value >* BSTree< Key, Value, Compare >::Iterator::operator->() const
{
  return &node_->data;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator& BSTree< Key, Value, Compare >::Iterator::operator++()
{
  if (node_->right != nullptr)
  {
    node_ = minNode(node_->right, fake_);
    return *this;
  }

  Node* parent = node_->parent;

  while (parent != nullptr && node_ == parent->right)
  {
    node_ = parent;
    parent = parent->parent;
  }

  node_ = parent == nullptr ? fake_ : parent;
  return *this;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator BSTree< Key, Value, Compare >::Iterator::operator++(int)
{
  Iterator result(*this);
  ++(*this);
  return result;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator& BSTree< Key, Value, Compare >::Iterator::operator--()
{
  if (node_ == fake_)
  {
    node_ = maxNode(fake_->left, fake_);
    return *this;
  }

  if (node_->left != nullptr)
  {
    node_ = maxNode(node_->left, fake_);
    return *this;
  }

  Node* parent = node_->parent;

  while (parent != nullptr && node_ == parent->left)
  {
    node_ = parent;
    parent = parent->parent;
  }

  node_ = parent == nullptr ? fake_ : parent;
  return *this;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator BSTree< Key, Value, Compare >::Iterator::operator--(int)
{
  Iterator result(*this);
  --(*this);
  return result;
}

template< class Key, class Value, class Compare >
bool BSTree< Key, Value, Compare >::Iterator::operator==(const Iterator& rhs) const noexcept
{
  return node_ == rhs.node_;
}

template< class Key, class Value, class Compare >
bool BSTree< Key, Value, Compare >::Iterator::operator!=(const Iterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::ConstIterator::ConstIterator():
  node_(nullptr),
  fake_(nullptr)
{}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::ConstIterator::ConstIterator(const Iterator& iter):
  node_(iter.node_),
  fake_(iter.fake_)
{}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::ConstIterator::ConstIterator(Node* node, Node* fake):
  node_(node),
  fake_(fake)
{}

template< class Key, class Value, class Compare >
const std::pair< Key, Value >& BSTree< Key, Value, Compare >::ConstIterator::operator*() const
{
  return node_->data;
}

template< class Key, class Value, class Compare >
const std::pair< Key, Value >* BSTree< Key, Value, Compare >::ConstIterator::operator->() const
{
  return &node_->data;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator& BSTree< Key, Value, Compare >::ConstIterator::operator++()
{
  Iterator iter(node_, fake_);
  ++iter;
  node_ = iter.node_;
  return *this;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator BSTree< Key, Value, Compare >::ConstIterator::operator++(int)
{
  ConstIterator result(*this);
  ++(*this);
  return result;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator& BSTree< Key, Value, Compare >::ConstIterator::operator--()
{
  Iterator iter(node_, fake_);
  --iter;
  node_ = iter.node_;
  return *this;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator BSTree< Key, Value, Compare >::ConstIterator::operator--(int)
{
  ConstIterator result(*this);
  --(*this);
  return result;
}

template< class Key, class Value, class Compare >
bool BSTree< Key, Value, Compare >::ConstIterator::operator==(const ConstIterator& rhs) const noexcept
{
  return node_ == rhs.node_;
}

template< class Key, class Value, class Compare >
bool BSTree< Key, Value, Compare >::ConstIterator::operator!=(const ConstIterator& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::BSTree():
  fake_(),
  size_(0),
  cmp_()
{}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  fake_(),
  size_(0),
  cmp_(other.cmp_)
{
  fake_.left = copySubtree(other.fake_.left, &fake_);
  size_ = other.size_;
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  fake_(),
  size_(0),
  cmp_(other.cmp_)
{
  fake_.left = other.fake_.left;

  if (fake_.left != nullptr)
  {
    fake_.left->parent = &fake_;
  }

  size_ = other.size_;
  other.fake_.left = nullptr;
  other.size_ = 0;
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  if (this != &other)
  {
    BSTree copy(other);
    swap(copy);
  }

  return *this;
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  if (this != &other)
  {
    clear();

    fake_.left = other.fake_.left;

    if (fake_.left != nullptr)
    {
      fake_.left->parent = &fake_;
    }

    size_ = other.size_;
    cmp_ = other.cmp_;

    other.fake_.left = nullptr;
    other.size_ = 0;
  }

  return *this;
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  using std::swap;

  swap(fake_.left, other.fake_.left);
  swap(size_, other.size_);
  swap(cmp_, other.cmp_);

  if (fake_.left != nullptr)
  {
    fake_.left->parent = &fake_;
  }

  if (other.fake_.left != nullptr)
  {
    other.fake_.left->parent = &other.fake_;
  }
}

template< class Key, class Value, class Compare >
bool BSTree< Key, Value, Compare >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Compare >
size_t BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::push(const Key& key, const Value& value)
{
  Node* parent = &fake_;
  Node* current = fake_.left;

  while (current != nullptr)
  {
    parent = current;

    if (cmp_(key, current->data.first))
    {
      current = current->left;
    }
    else if (cmp_(current->data.first, key))
    {
      current = current->right;
    }
    else
    {
      throw std::logic_error("key already exists");
    }
  }

  Node* node = new Node(key, value);
  node->parent = parent;

  if (parent == &fake_)
  {
    fake_.left = node;
  }
  else if (cmp_(key, parent->data.first))
  {
    parent->left = node;
  }
  else
  {
    parent->right = node;
  }

  ++size_;
}

template< class Key, class Value, class Compare >
Value BSTree< Key, Value, Compare >::drop(const Key& key)
{
  Node* node = findNode(key);

  if (node == nullptr)
  {
    throw std::logic_error("key not found");
  }

  Value result = node->data.second;

  if (node->left == nullptr)
  {
    transplant(node, node->right);
  }
  else if (node->right == nullptr)
  {
    transplant(node, node->left);
  }
  else
  {
    Node* replacement = minNode(node->right, &fake_);

    if (replacement->parent != node)
    {
      transplant(replacement, replacement->right);
      replacement->right = node->right;
      replacement->right->parent = replacement;
    }

    transplant(node, replacement);
    replacement->left = node->left;
    replacement->left->parent = replacement;
  }

  delete node;
  --size_;

  return result;
}

template< class Key, class Value, class Compare >
bool BSTree< Key, Value, Compare >::has(const Key& key) const
{
  return findNode(key) != nullptr;
}

template< class Key, class Value, class Compare >
Value& BSTree< Key, Value, Compare >::at(const Key& key)
{
  Node* node = findNode(key);

  if (node == nullptr)
  {
    throw std::logic_error("key not found");
  }

  return node->data.second;
}

template< class Key, class Value, class Compare >
const Value& BSTree< Key, Value, Compare >::at(const Key& key) const
{
  Node* node = findNode(key);

  if (node == nullptr)
  {
    throw std::logic_error("key not found");
  }

  return node->data.second;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator BSTree< Key, Value, Compare >::begin() noexcept
{
  return Iterator(minNode(fake_.left, &fake_), &fake_);
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator BSTree< Key, Value, Compare >::end() noexcept
{
  return Iterator(&fake_, &fake_);
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator BSTree< Key, Value, Compare >::begin() const noexcept
{
  return cbegin();
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator BSTree< Key, Value, Compare >::end() const noexcept
{
  return cend();
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  Node* fake = const_cast< Node* >(&fake_);
  return ConstIterator(minNode(fake_.left, fake), fake);
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator BSTree< Key, Value, Compare >::cend() const noexcept
{
  Node* fake = const_cast< Node* >(&fake_);
  return ConstIterator(fake, fake);
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator BSTree< Key, Value, Compare >::find(const Key& key) noexcept
{
  Node* node = findNode(key);
  return Iterator(node == nullptr ? &fake_ : node, &fake_);
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::ConstIterator BSTree< Key, Value, Compare >::find(const Key& key) const noexcept
{
  Node* fake = const_cast< Node* >(&fake_);
  Node* node = findNode(key);
  return ConstIterator(node == nullptr ? fake : node, fake);
}

template< class Key, class Value, class Compare >
size_t BSTree< Key, Value, Compare >::height() const noexcept
{
  return heightNode(fake_.left);
}

template< class Key, class Value, class Compare >
size_t BSTree< Key, Value, Compare >::height(ConstIterator pos) const noexcept
{
  if (pos.node_ == const_cast< Node* >(&fake_))
  {
    return 0;
  }

  return heightNode(pos.node_);
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator BSTree< Key, Value, Compare >::rotateLeft(Iterator rising)
{
  Node* node = rising.node_;

  if (node == &fake_ || node == nullptr || node->parent == nullptr)
  {
    throw std::logic_error("invalid rotation");
  }

  Node* parent = node->parent;

  if (parent == &fake_ || parent->right != node)
  {
    throw std::logic_error("invalid rotation");
  }

  Node* grandparent = parent->parent;
  parent->right = node->left;

  if (node->left != nullptr)
  {
    node->left->parent = parent;
  }

  node->left = parent;
  parent->parent = node;
  node->parent = grandparent;

  if (grandparent->left == parent)
  {
    grandparent->left = node;
  }
  else
  {
    grandparent->right = node;
  }

  return Iterator(parent, &fake_);
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Iterator BSTree< Key, Value, Compare >::rotateRight(Iterator rising)
{
  Node* node = rising.node_;

  if (node == &fake_ || node == nullptr || node->parent == nullptr)
  {
    throw std::logic_error("invalid rotation");
  }

  Node* parent = node->parent;

  if (parent == &fake_ || parent->left != node)
  {
    throw std::logic_error("invalid rotation");
  }

  Node* grandparent = parent->parent;
  parent->left = node->right;

  if (node->right != nullptr)
  {
    node->right->parent = parent;
  }

  node->right = parent;
  parent->parent = node;
  node->parent = grandparent;

  if (grandparent->left == parent)
  {
    grandparent->left = node;
  }
  else
  {
    grandparent->right = node;
  }

  return Iterator(parent, &fake_);
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::clear() noexcept
{
  deleteSubtree(fake_.left);
  fake_.left = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Node* BSTree< Key, Value, Compare >::findNode(const Key& key) const noexcept
{
  Node* current = fake_.left;

  while (current != nullptr)
  {
    if (cmp_(key, current->data.first))
    {
      current = current->left;
    }
    else if (cmp_(current->data.first, key))
    {
      current = current->right;
    }
    else
    {
      return current;
    }
  }

  return nullptr;
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::transplant(Node* old_node, Node* new_node) noexcept
{
  if (old_node->parent->left == old_node)
  {
    old_node->parent->left = new_node;
  }
  else
  {
    old_node->parent->right = new_node;
  }

  if (new_node != nullptr)
  {
    new_node->parent = old_node->parent;
  }
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Node* BSTree< Key, Value, Compare >::minNode(Node* node, Node* fake) noexcept
{
  if (node == nullptr)
  {
    return fake;
  }

  while (node->left != nullptr)
  {
    node = node->left;
  }

  return node;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Node* BSTree< Key, Value, Compare >::maxNode(Node* node, Node* fake) noexcept
{
  if (node == nullptr)
  {
    return fake;
  }

  while (node->right != nullptr)
  {
    node = node->right;
  }

  return node;
}

template< class Key, class Value, class Compare >
size_t BSTree< Key, Value, Compare >::heightNode(Node* node) noexcept
{
  if (node == nullptr)
  {
    return 0;
  }

  size_t left_height = heightNode(node->left);
  size_t right_height = heightNode(node->right);

  return left_height > right_height ? left_height + 1 : right_height + 1;
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::deleteSubtree(Node* node) noexcept
{
  if (node == nullptr)
  {
    return;
  }

  deleteSubtree(node->left);
  deleteSubtree(node->right);
  delete node;
}

template< class Key, class Value, class Compare >
typename BSTree< Key, Value, Compare >::Node* BSTree< Key, Value, Compare >::copySubtree(Node* node, Node* parent)
{
  if (node == nullptr)
  {
    return nullptr;
  }

  Node* result = new Node(node->data.first, node->data.second);
  result->parent = parent;

  try
  {
    result->left = copySubtree(node->left, result);
    result->right = copySubtree(node->right, result);
  }
  catch (...)
  {
    deleteSubtree(result);
    throw;
  }

  return result;
}
}

#endif
