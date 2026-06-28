#include "avltree.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "question.hpp"
#include "tag.hpp"

template< class Key, class Value, class Compare >
petrov::AvlTree< Key, Value, Compare >::Node::Node(
  const Key& key, const Value& value, Node* parentNode):
  data(key, value),
  parent(parentNode),
  left(nullptr),
  right(nullptr),
  height(1)
{}

template< class Key, class Value, class Compare >
petrov::AvlTree< Key, Value, Compare >::AvlTree():
  fake_(new Node(Key(), Value(), nullptr)),
  size_(0),
  compare_()
{
  fake_->left = nullptr;
  fake_->right = nullptr;
}

template< class Key, class Value, class Compare >
petrov::AvlTree< Key, Value, Compare >::AvlTree(const AvlTree& other):
  fake_(new Node(Key(), Value(), nullptr)),
  size_(other.size_),
  compare_(other.compare_)
{
  fake_->left = copy(other.root(), fake_);
  fake_->right = nullptr;
}

template< class Key, class Value, class Compare >
petrov::AvlTree< Key, Value, Compare >::AvlTree(AvlTree&& other) noexcept:
  fake_(other.fake_),
  size_(other.size_),
  compare_(other.compare_)
{
  other.fake_ = new Node(Key(), Value(), nullptr);
  other.fake_->left = nullptr;
  other.fake_->right = nullptr;
  other.size_ = 0;
}

template< class Key, class Value, class Compare >
petrov::AvlTree< Key, Value, Compare >::~AvlTree()
{
  clear(root());
  delete fake_;
}

template< class Key, class Value, class Compare >
petrov::AvlTree< Key, Value, Compare >&
petrov::AvlTree< Key, Value, Compare >::operator=(const AvlTree& other)
{
  if (this != std::addressof(other)) {
    clear(root());
    fake_->left = copy(other.root(), fake_);
    size_ = other.size_;
    compare_ = other.compare_;
  }
  return *this;
}

template< class Key, class Value, class Compare >
petrov::AvlTree< Key, Value, Compare >&
petrov::AvlTree< Key, Value, Compare >::operator=(AvlTree&& other) noexcept
{
  if (this != std::addressof(other)) {
    clear(root());
    delete fake_;
    fake_ = other.fake_;
    size_ = other.size_;
    compare_ = other.compare_;
    other.fake_ = new Node(Key(), Value(), nullptr);
    other.fake_->left = nullptr;
    other.fake_->right = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::Node*
petrov::AvlTree< Key, Value, Compare >::root() const noexcept
{
  return fake_->left;
}

template< class Key, class Value, class Compare >
void petrov::AvlTree< Key, Value, Compare >::setRoot(Node* node) noexcept
{
  fake_->left = node;
  if (node != nullptr) {
    node->parent = fake_;
  }
}

template< class Key, class Value, class Compare >
void petrov::AvlTree< Key, Value, Compare >::clear(Node* node) noexcept
{
  if (node == nullptr) {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node;
}

template< class Key, class Value, class Compare >
void petrov::AvlTree< Key, Value, Compare >::clear() noexcept
{
  clear(root());
  fake_->left = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::Node*
petrov::AvlTree< Key, Value, Compare >::copy(Node* node, Node* parent)
{
  if (node == nullptr) {
    return nullptr;
  }
  Node* newNode = new Node(node->data.first, node->data.second, parent);
  newNode->height = node->height;
  newNode->left = copy(node->left, newNode);
  newNode->right = copy(node->right, newNode);
  return newNode;
}

template< class Key, class Value, class Compare >
std::size_t petrov::AvlTree< Key, Value, Compare >::count(Node* node) const noexcept
{
  if (node == nullptr) {
    return 0;
  }
  return count(node->left) + count(node->right) + 1;
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::Node*
petrov::AvlTree< Key, Value, Compare >::findNode(const Key& key) const noexcept
{
  Node* current = root();
  while (current != nullptr) {
    if (compare_(key, current->data.first)) {
      current = current->left;
    } else if (compare_(current->data.first, key)) {
      current = current->right;
    } else {
      return current;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::Node*
petrov::AvlTree< Key, Value, Compare >::minimum(Node* node) const noexcept
{
  if (node == nullptr) {
    return nullptr;
  }
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

template< class Key, class Value, class Compare >
int petrov::AvlTree< Key, Value, Compare >::nodeHeight(Node* node) const noexcept
{
  if (node == nullptr) {
    return 0;
  }
  return node->height;
}

template< class Key, class Value, class Compare >
void petrov::AvlTree< Key, Value, Compare >::updateHeight(Node* node) noexcept
{
  const int leftHeight = nodeHeight(node->left);
  const int rightHeight = nodeHeight(node->right);
  node->height = std::max(leftHeight, rightHeight) + 1;
}

template< class Key, class Value, class Compare >
int petrov::AvlTree< Key, Value, Compare >::balanceFactor(Node* node) const noexcept
{
  if (node == nullptr) {
    return 0;
  }
  return nodeHeight(node->left) - nodeHeight(node->right);
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::Node*
petrov::AvlTree< Key, Value, Compare >::rotateLeftNode(Node* node)
{
  Node* newRoot = node->right;
  node->right = newRoot->left;
  if (newRoot->left != nullptr) {
    newRoot->left->parent = node;
  }
  newRoot->parent = node->parent;
  if (node->parent->left == node) {
    node->parent->left = newRoot;
  } else {
    node->parent->right = newRoot;
  }
  newRoot->left = node;
  node->parent = newRoot;
  updateHeight(node);
  updateHeight(newRoot);
  return newRoot;
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::Node*
petrov::AvlTree< Key, Value, Compare >::rotateRightNode(Node* node)
{
  Node* newRoot = node->left;
  node->left = newRoot->right;
  if (newRoot->right != nullptr) {
    newRoot->right->parent = node;
  }
  newRoot->parent = node->parent;
  if (node->parent->left == node) {
    node->parent->left = newRoot;
  } else {
    node->parent->right = newRoot;
  }
  newRoot->right = node;
  node->parent = newRoot;
  updateHeight(node);
  updateHeight(newRoot);
  return newRoot;
}

template< class Key, class Value, class Compare >
void petrov::AvlTree< Key, Value, Compare >::rebalanceFrom(Node* node)
{
  while (node != fake_) {
    updateHeight(node);
    Node* parent = node->parent;
    const int balance = balanceFactor(node);
    if (balance > 1) {
      if (balanceFactor(node->left) < 0) {
        rotateLeftNode(node->left);
      }
      rotateRightNode(node);
    } else if (balance < -1) {
      if (balanceFactor(node->right) > 0) {
        rotateRightNode(node->right);
      }
      rotateLeftNode(node);
    }
    node = parent;
  }
}

template< class Key, class Value, class Compare >
void petrov::AvlTree< Key, Value, Compare >::push(const Key& key, const Value& value)
{
  if (root() == nullptr) {
    setRoot(new Node(key, value, fake_));
    size_ = size_ + 1;
    return;
  }
  Node* parent = fake_;
  Node* current = root();
  while (current != nullptr) {
    parent = current;
    if (compare_(key, current->data.first)) {
      current = current->left;
    } else if (compare_(current->data.first, key)) {
      current = current->right;
    } else {
      current->data.second = value;
      return;
    }
  }
  Node* newNode = new Node(key, value, parent);
  if (compare_(key, parent->data.first)) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }
  size_ = size_ + 1;
  rebalanceFrom(parent);
}

template< class Key, class Value, class Compare >
Value& petrov::AvlTree< Key, Value, Compare >::get(const Key& key)
{
  Node* node = findNode(key);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return node->data.second;
}

template< class Key, class Value, class Compare >
const Value& petrov::AvlTree< Key, Value, Compare >::get(const Key& key) const
{
  Node* node = findNode(key);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return node->data.second;
}

template< class Key, class Value, class Compare >
Value petrov::AvlTree< Key, Value, Compare >::drop(const Key& key)
{
  Node* node = findNode(key);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  Value result = node->data.second;
  if (node->left != nullptr && node->right != nullptr) {
    Node* successor = minimum(node->right);
    const_cast< Key& >(node->data.first) = successor->data.first;
    node->data.second = successor->data.second;
    node = successor;
  }
  Node* child = (node->left != nullptr) ? node->left : node->right;
  if (child != nullptr) {
    child->parent = node->parent;
  }
  Node* parent = node->parent;
  if (parent->left == node) {
    parent->left = child;
  } else {
    parent->right = child;
  }
  delete node;
  size_ = size_ - 1;
  rebalanceFrom(parent);
  return result;
}

template< class Key, class Value, class Compare >
bool petrov::AvlTree< Key, Value, Compare >::has(const Key& key) const noexcept
{
  return findNode(key) != nullptr;
}

template< class Key, class Value, class Compare >
bool petrov::AvlTree< Key, Value, Compare >::empty() const noexcept
{
  return root() == nullptr;
}

template< class Key, class Value, class Compare >
std::size_t petrov::AvlTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::iterator
petrov::AvlTree< Key, Value, Compare >::begin() noexcept
{
  return iterator(minimum(root()), fake_);
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::iterator
petrov::AvlTree< Key, Value, Compare >::end() noexcept
{
  return iterator(nullptr, fake_);
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::const_iterator
petrov::AvlTree< Key, Value, Compare >::begin() const noexcept
{
  return const_iterator(minimum(root()), fake_);
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::const_iterator
petrov::AvlTree< Key, Value, Compare >::end() const noexcept
{
  return const_iterator(nullptr, fake_);
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::iterator
petrov::AvlTree< Key, Value, Compare >::find(const Key& key) noexcept
{
  return iterator(findNode(key), fake_);
}

template< class Key, class Value, class Compare >
typename petrov::AvlTree< Key, Value, Compare >::const_iterator
petrov::AvlTree< Key, Value, Compare >::find(const Key& key) const noexcept
{
  return const_iterator(findNode(key), fake_);
}

template< class Key, class Value, class Compare >
std::size_t petrov::AvlTree< Key, Value, Compare >::height() const noexcept
{
  return static_cast< std::size_t >(nodeHeight(root()));
}

template< class Key, class Value >
petrov::AvlConstIterator< Key, Value >::AvlConstIterator() noexcept:
  node_(nullptr),
  fake_(nullptr)
{}

template< class Key, class Value >
petrov::AvlConstIterator< Key, Value >::AvlConstIterator(Node* node, Node* fake) noexcept:
  node_(node),
  fake_(fake)
{}

template< class Key, class Value >
const typename petrov::AvlConstIterator< Key, Value >::value_type&
petrov::AvlConstIterator< Key, Value >::operator*() const noexcept
{
  return node_->data;
}

template< class Key, class Value >
const typename petrov::AvlConstIterator< Key, Value >::value_type*
petrov::AvlConstIterator< Key, Value >::operator->() const noexcept
{
  return std::addressof(node_->data);
}

template< class Key, class Value >
petrov::AvlConstIterator< Key, Value >& petrov::AvlConstIterator< Key, Value >::operator++() noexcept
{
  if (node_ == nullptr) {
    return *this;
  }
  if (node_->right != nullptr) {
    node_ = node_->right;
    while (node_->left != nullptr) {
      node_ = node_->left;
    }
  } else {
    Node* parent = node_->parent;
    while (parent != fake_ && node_ == parent->right) {
      node_ = parent;
      parent = parent->parent;
    }
    node_ = (parent == fake_) ? nullptr : parent;
  }
  return *this;
}

template< class Key, class Value >
petrov::AvlConstIterator< Key, Value > petrov::AvlConstIterator< Key, Value >::operator++(int) noexcept
{
  AvlConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
bool petrov::AvlConstIterator< Key, Value >::operator==(const AvlConstIterator& other) const noexcept
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool petrov::AvlConstIterator< Key, Value >::operator!=(const AvlConstIterator& other) const noexcept
{
  return node_ != other.node_;
}

template< class Key, class Value >
petrov::AvlIterator< Key, Value >::AvlIterator() noexcept:
  node_(nullptr),
  fake_(nullptr)
{}

template< class Key, class Value >
petrov::AvlIterator< Key, Value >::AvlIterator(Node* node, Node* fake) noexcept:
  node_(node),
  fake_(fake)
{}

template< class Key, class Value >
typename petrov::AvlIterator< Key, Value >::value_type& petrov::AvlIterator< Key, Value >::operator*() const noexcept
{
  return node_->data;
}

template< class Key, class Value >
typename petrov::AvlIterator< Key, Value >::value_type* petrov::AvlIterator< Key, Value >::operator->() const noexcept
{
  return std::addressof(node_->data);
}

template< class Key, class Value >
petrov::AvlIterator< Key, Value >& petrov::AvlIterator< Key, Value >::operator++() noexcept
{
  AvlConstIterator< Key, Value > temp(node_, fake_);
  ++temp;
  node_ = temp.node_;
  return *this;
}

template< class Key, class Value >
petrov::AvlIterator< Key, Value > petrov::AvlIterator< Key, Value >::operator++(int) noexcept
{
  AvlIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
bool petrov::AvlIterator< Key, Value >::operator==(const AvlIterator& other) const noexcept
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool petrov::AvlIterator< Key, Value >::operator!=(const AvlIterator& other) const noexcept
{
  return node_ != other.node_;
}

template< class Key, class Value >
petrov::AvlIterator< Key, Value >::operator petrov::AvlConstIterator< Key, Value >() const noexcept
{
  return AvlConstIterator< Key, Value >(node_, fake_);
}

template class petrov::AvlConstIterator< std::string, petrov::Question >;
template class petrov::AvlIterator< std::string, petrov::Question >;
template class petrov::AvlTree< std::string, petrov::Question >;

template class petrov::AvlConstIterator< std::string, petrov::Tag >;
template class petrov::AvlIterator< std::string, petrov::Tag >;
template class petrov::AvlTree< std::string, petrov::Tag >;

