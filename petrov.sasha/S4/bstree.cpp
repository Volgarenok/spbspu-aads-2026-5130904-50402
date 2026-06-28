#include "bstree.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>

template< class K, class V, class C >
petrov::BSTree< K, V, C >::Node::Node(const K& key, const V& value, Node* parentNode):
  data(key, value),
  parent(parentNode),
  left(nullptr),
  right(nullptr)
{}

template< class Key, class Value, class Compare >
petrov::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  m_fake(new Node(Key(), Value(), nullptr)),
  m_compare(other.m_compare)
{
  try {
    m_fake->left = copy(other.root(), m_fake);
  } catch (...) {
    delete m_fake;
    throw;
  }
  m_fake->right = nullptr;
}

template< class Key, class Value, class Compare >
petrov::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  m_fake(other.m_fake),
  m_compare(other.m_compare)
{
  other.m_fake = nullptr;
}

template< class Key, class Value, class Compare >
void petrov::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(m_fake, other.m_fake);
  std::swap(m_compare, other.m_compare);
}
template< class Key, class Value, class Compare >
petrov::BSTree< Key, Value, Compare >& petrov::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  if (this != &other) {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
petrov::BSTree< Key, Value, Compare >& petrov::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  if (this != &other) {
    clear(root());
    delete m_fake;
    m_fake = other.m_fake;
    m_compare = other.m_compare;
    other.m_fake = nullptr;
  }
  return *this;
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::Node*
petrov::BSTree< Key, Value, Compare >::copy(Node* node, Node* parent)
{
  if (node == nullptr) {
    return nullptr;
  }
  Node* newNode = new Node(node->data.first, node->data.second, parent);
  try {
    newNode->left = copy(node->left, newNode);
    newNode->right = copy(node->right, newNode);
  } catch (...) {
    clear(newNode);
    throw;
  }
  return newNode;
}

template< class Key, class Value >
petrov::BSTConstIterator< Key, Value >::BSTConstIterator() noexcept:
  m_node(nullptr),
  m_fake(nullptr)
{}

template< class Key, class Value >
petrov::BSTConstIterator< Key, Value >::BSTConstIterator(
    Node* node,
    Node* fake) noexcept:
  m_node(node),
  m_fake(fake)
{}

template< class Key, class Value >
const typename petrov::BSTConstIterator< Key, Value >::value_type&
petrov::BSTConstIterator< Key, Value >::operator*() const noexcept
{
  return m_node->data;
}

template< class Key, class Value >
const typename petrov::BSTConstIterator< Key, Value >::value_type*
petrov::BSTConstIterator< Key, Value >::operator->() const noexcept
{
  return &m_node->data;
}

template< class Key, class Value >
petrov::BSTConstIterator< Key, Value >&
petrov::BSTConstIterator< Key, Value >::operator++() noexcept
{
  if (m_node == nullptr) {
    return *this;
  }
  if (m_node->right != nullptr) {
    m_node = m_node->right;
    while (m_node->left != nullptr) {
      m_node = m_node->left;
    }
  } else {
    Node* parent = m_node->parent;
    while (parent != m_fake && m_node == parent->right) {
      m_node = parent;
      parent = parent->parent;
    }
    m_node = (parent == m_fake) ? nullptr : parent;
  }
  return *this;
}

template< class Key, class Value >
petrov::BSTConstIterator< Key, Value >
petrov::BSTConstIterator< Key, Value >::operator++(int) noexcept
{
  BSTConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
bool petrov::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator& other) const noexcept
{
  return m_node == other.m_node;
}

template< class Key, class Value >
bool petrov::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator& other) const noexcept
{
  return m_node != other.m_node;
}

template< class Key, class Value >
petrov::BSTIterator< Key, Value >::BSTIterator() noexcept:
  m_node(nullptr),
  m_fake(nullptr)
{}

template< class Key, class Value >
petrov::BSTIterator< Key, Value >::BSTIterator(Node* node, Node* fake) noexcept:
  m_node(node),
  m_fake(fake)
{}

template< class Key, class Value >
typename petrov::BSTIterator< Key, Value >::value_type& petrov::BSTIterator< Key, Value >::operator*() const noexcept
{
  return m_node->data;
}

template< class Key, class Value >
typename petrov::BSTIterator< Key, Value >::value_type*
petrov::BSTIterator< Key, Value >::operator->() const noexcept
{
  return &m_node->data;
}

template< class Key, class Value >
petrov::BSTIterator< Key, Value >&
petrov::BSTIterator< Key, Value >::operator++() noexcept
{
  BSTConstIterator< Key, Value > temp(m_node, m_fake);
  ++temp;
  m_node = temp.m_node;
  return *this;
}

template< class Key, class Value >
petrov::BSTIterator< Key, Value > petrov::BSTIterator< Key, Value >::operator++(int) noexcept
{
  BSTIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
bool petrov::BSTIterator< Key, Value >::operator==(const BSTIterator& other) const noexcept
{
  return m_node == other.m_node;
}

template< class Key, class Value >
bool petrov::BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const noexcept
{
  return m_node != other.m_node;
}

template< class Key, class Value >
petrov::BSTIterator< Key, Value >::operator petrov::BSTConstIterator< Key, Value >() const noexcept
{
  return BSTConstIterator< Key, Value >(m_node, m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::Node* petrov::BSTree< Key, Value, Compare >::root() const noexcept
{
  return m_fake != nullptr ? m_fake->left : nullptr;
}

template< class Key, class Value, class Compare >
void petrov::BSTree< Key, Value, Compare >::setRoot(Node* node) noexcept
{
  m_fake->left = node;
  if (node != nullptr) {
    node->parent = m_fake;
  }
}

template< class Key, class Value, class Compare >
petrov::BSTree< Key, Value, Compare >::BSTree():
  m_fake(new Node(Key(), Value(), nullptr)),
  m_compare()
{
  m_fake->left = nullptr;
  m_fake->right = nullptr;
}

template< class Key, class Value, class Compare >
petrov::BSTree< Key, Value, Compare >::~BSTree()
{
  if (m_fake != nullptr) {
    clear(root());
    delete m_fake;
  }
}

template< class Key, class Value, class Compare >
void petrov::BSTree< Key, Value, Compare >::clear(Node* node) noexcept
{
  if (node == nullptr) {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node;
}

template< class Key, class Value, class Compare >
std::size_t petrov::BSTree< Key, Value, Compare >::height(Node* node) const noexcept
{
  if (node == nullptr) {
    return 0;
  }

  std::size_t leftHeight = height(node->left);
  std::size_t rightHeight = height(node->right);

  return std::max(leftHeight, rightHeight) + 1;
}

template< class Key, class Value, class Compare >
std::size_t petrov::BSTree< Key, Value, Compare >::height() const noexcept
{
  return height(root());
}

template< class Key, class Value, class Compare >
std::size_t petrov::BSTree< Key, Value, Compare >::height(const_iterator it) const noexcept
{
  return height(it.m_node);
}

template< class Key, class Value, class Compare >
void petrov::BSTree< Key, Value, Compare >::push(const Key& key, const Value& value)
{
  if (root() == nullptr)
  {
    setRoot(new Node(key, value, m_fake));
    return;
  }
  Node* parent = m_fake;
  Node* current = root();
  while (current != nullptr) {
    parent = current;
    if (m_compare(key, current->data.first)) {
      current = current->left;
    } else if (m_compare(current->data.first, key)) {
      current = current->right;
    } else {
      current->data.second = value;
      return;
    }
  }
  Node* newNode = new Node(key, value, parent);
  if (m_compare(key, parent->data.first)) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }
}
template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::Node*
petrov::BSTree< Key, Value, Compare >::findNode(const Key& key) const noexcept
{
  Node* current = root();
  while (current != nullptr){
    if (m_compare(key, current->data.first)) {
      current = current->left;
    } else if (m_compare(current->data.first, key)) {
      current = current->right;
    } else {
      return current;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
Value& petrov::BSTree< Key, Value, Compare >::get(const Key& key)
{
  Node* node = findNode(key);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return node->data.second;
}

template< class Key, class Value, class Compare >
const Value& petrov::BSTree< Key, Value, Compare >::get(const Key& key) const
{
  Node* node = findNode(key);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  return node->data.second;
}

template< class Key, class Value, class Compare >
bool petrov::BSTree< Key, Value, Compare >::has(const Key& key) const noexcept
{
  return findNode(key) != nullptr;
}

template< class Key, class Value, class Compare >
bool petrov::BSTree< Key, Value, Compare >::empty() const noexcept
{
  return root() == nullptr;
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::Node*
petrov::BSTree< Key, Value, Compare >::rotateLeftNode(Node* node)
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
  return newRoot;
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::Node*
petrov::BSTree< Key, Value, Compare >::rotateRightNode(Node* node)
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
  return newRoot;
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::const_iterator
petrov::BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
{
  return const_iterator(rotateLeftNode(it.m_node), m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::const_iterator
petrov::BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
{
  return const_iterator(rotateRightNode(it.m_node), m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::const_iterator
petrov::BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
{
  Node* node = it.m_node;
  if (node == nullptr || node->right == nullptr || node->right->left == nullptr) {
    return it;
  }
  rotateRightNode(node->right);
  return const_iterator(rotateLeftNode(node), m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::const_iterator
petrov::BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
{
  Node* node = it.m_node;
  if (node == nullptr || node->left == nullptr || node->left->right == nullptr) {
    return it;
  }
  rotateLeftNode(node->left);
  return const_iterator(rotateRightNode(node), m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::Node*
petrov::BSTree< Key, Value, Compare >::minimum(Node* node) const noexcept
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
typename petrov::BSTree< Key, Value, Compare >::iterator
petrov::BSTree< Key, Value, Compare >::begin() noexcept
{
  return iterator(minimum(root()), m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::iterator
petrov::BSTree< Key, Value, Compare >::end() noexcept {
  return iterator(nullptr, m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::const_iterator
petrov::BSTree< Key, Value, Compare >::begin() const noexcept
{
  return const_iterator(minimum(root()), m_fake);
}

template< class Key, class Value, class Compare >
typename petrov::BSTree< Key, Value, Compare >::const_iterator
petrov::BSTree< Key, Value, Compare >::end() const noexcept
{
  return const_iterator(nullptr, m_fake);
}

template< class Key, class Value, class Compare >
Value petrov::BSTree< Key, Value, Compare >::drop(const Key& key)
{
  Node* node = findNode(key);
  if (node == nullptr) {
    throw std::out_of_range("Key not found");
  }
  Value result = node->data.second;
  eraseNode(node);
  return result;
}

template< class Key, class Value, class Compare >
void petrov::BSTree< Key, Value, Compare >::eraseNode(Node* node) noexcept
{
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
  if (node->parent->left == node) {
    node->parent->left = child;
  } else {
    node->parent->right = child;
  }
  delete node;
}

template class petrov::BSTConstIterator< int, std::string >;
template class petrov::BSTIterator< int, std::string >;
template class petrov::BSTree< int, std::string >;

template class petrov::BSTConstIterator<
  std::string,
  petrov::BSTree< int, std::string >
>;
template class petrov::BSTIterator<
  std::string,
  petrov::BSTree< int, std::string >
>;
template class petrov::BSTree<
  std::string,
  petrov::BSTree< int, std::string >
>;


