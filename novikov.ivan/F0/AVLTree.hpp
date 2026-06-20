#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <string>
#include <algorithm>

namespace novikov
{
  template< class Key, class Value, class Compare >
  class AVLTree;
}

namespace novikov
{
  template< class Key, class Value >
  struct Node
  {
    std::pair< Key, Value > data_;
    Node* left_ = nullptr;
    Node* right_ = nullptr;
    size_t height_ = 0;
    Node* parent_ = nullptr;
  };

  template< class Key, class Value >
  class AVLIterator {
    using Node = novikov::Node< Key, Value >;

  public:
    explicit AVLIterator(Node* other);
    bool operator==(const AVLIterator< Key, Value >& other) const;
    bool operator!=(const AVLIterator< Key, Value >& other) const;
    AVLIterator< Key, Value >& operator++();
    std::pair< Key, Value >& operator*();

  private:
    Node* curr_;
    template< class K, class V, class C >
    friend class AVLTree;
  };

  template< class Key, class Value >
  class AVLConstIterator
  {
    using Node = novikov::Node< Key, Value >;

  public:
    explicit AVLConstIterator(const Node* other);
    bool operator==(const AVLConstIterator< Key, Value >& other) const;
    bool operator!=(const AVLConstIterator< Key, Value >& other) const;
    AVLConstIterator< Key, Value >& operator++();
    const std::pair< Key, Value >& operator*();

  private:
    const Node* curr_;
    template< class K, class V, class C >
    friend class AVLTree;
  };

  template< class Key, class Value, class Compare >
  class AVLTree
  {
    using Node = novikov::Node< Key, Value >;
    friend class AVLIterator< Key, Value >;
    friend class AVLConstIterator< Key, Value >;

  public:
    AVLTree();
    ~AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    AVLTree& operator=(AVLTree other);
    Value& operator[](const Key& k);

    void push(Key k, Value v);
    const Value& get(const Key& k) const;
    void drop(const Key& k);
    bool has(const Key& k) const;

    using iterator = AVLIterator< Key, Value >;
    using const_iterator = AVLConstIterator< Key, Value >;
    iterator rotateLeft(iterator it);
    iterator rotateRight(iterator it);
    iterator rotateLargeLeft(iterator it);
    iterator rotateLargeRight(iterator it);

    size_t height() const;
    size_t height(const_iterator it) const;
    void setName(std::string name);
    std::string getName();
    AVLIterator< Key, Value > begin();
    AVLConstIterator< Key, Value > cbegin();
    AVLIterator< Key, Value > end();
    AVLConstIterator< Key, Value > cend();

  private:
    Node* fakeroot_;
    Compare compare_;
    std::string name_;

    void clear(Node* fakeroot);
    Node* copyNodes(Node* other, Node* parent);
    void swap(AVLTree& other) noexcept;
    Value& insertNode(Node*& node, Key k, Value v, bool isOperator, Node* parent = nullptr);
    void deleteNode(Node*& node, const Key& k);
    void updateHeight(Node* node);
    size_t getHeight(const Node* node) const;

    int getBalance(const Node* node) const;
    void balanceNode(Node*& node);
    void internalRotateLeft(Node*& node);
    void internalRotateRight(Node*& node);
  };

  template< class Key, class Value >
  Node< Key, Value >* fallLeft(Node< Key, Value >* node);
}

template< class Key, class Value, class Compare >
novikov::AVLTree< Key, Value, Compare >::AVLTree():
    fakeroot_(new Node{{Key(), Value()}}),
    compare_(Compare())
{}

template< class Key, class Value, class Compare >
novikov::AVLTree< Key, Value, Compare >::~AVLTree()
{
  clear(fakeroot_);
}

template< class Key, class Value, class Compare >
novikov::AVLTree< Key, Value, Compare >::AVLTree(const AVLTree& other):
    fakeroot_(new Node{{Key(), Value()}}),
    compare_(other.compare_),
    name_(other.name_)
{
  fakeroot_->left_ = copyNodes(other.fakeroot_->left_, fakeroot_);
}

template< class Key, class Value, class Compare >
novikov::AVLTree< Key, Value, Compare >::AVLTree(AVLTree&& other) noexcept:
    fakeroot_(other.fakeroot_),
    compare_(std::move(other.compare_)),
    name_(std::move(other.name_))
{
  other.fakeroot_ = nullptr;
}

template< class Key, class Value, class Compare >
novikov::AVLTree< Key, Value, Compare >& novikov::AVLTree< Key, Value, Compare >::operator=(AVLTree other)
{
  swap(other);
  return *this;
}

template< class Key, class Value, class Compare >
int novikov::AVLTree< Key, Value, Compare >::getBalance(const Node* node) const
{
  if (!node) return 0;
  return static_cast<int>(getHeight(node->left_)) - static_cast<int>(getHeight(node->right_));
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::internalRotateLeft(Node*& node)
{
  Node* rightChild = node->right_;
  node->right_ = rightChild->left_;
  if (node->right_)
  {
    node->right_->parent_ = node;
  }
  rightChild->left_ = node;
  rightChild->parent_ = node->parent_;
  node->parent_ = rightChild;

  updateHeight(node);
  updateHeight(rightChild);
  node = rightChild;
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::internalRotateRight(Node*& node)
{
  Node* leftChild = node->left_;
  node->left_ = leftChild->right_;
  if (node->left_)
  {
    node->left_->parent_ = node;
  }
  leftChild->right_ = node;
  leftChild->parent_ = node->parent_;
  node->parent_ = leftChild;

  updateHeight(node);
  updateHeight(leftChild);
  node = leftChild;
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::balanceNode(Node*& node)
{
  if (!node) return;

  updateHeight(node);
  int balance = getBalance(node);

  if (balance > 1)
  {
    if (getBalance(node->left_) < 0)
    {
      internalRotateLeft(node->left_);
    }
    internalRotateRight(node);
  }
  else if (balance < -1)
  {
    if (getBalance(node->right_) > 0)
    {
      internalRotateRight(node->right_);
    }
    internalRotateLeft(node);
  }
}

template< class Key, class Value, class Compare >
Value& novikov::AVLTree< Key, Value, Compare >::insertNode(Node*& node, Key k, Value v, bool isOperator, Node* parent)
{
  if (node == nullptr)
  {
    node = new Node();
    node->data_ = {k, v};
    node->height_ = 1;
    node->parent_ = parent;
    return node->data_.second;
  }

  Value* result = nullptr;

  if (compare_(k, node->data_.first))
  {
    result = &insertNode(node->left_, k, v, isOperator, node);
  }
  else if (compare_(node->data_.first, k))
  {
    result = &insertNode(node->right_, k, v, isOperator, node);
  }
  else
  {
    if (isOperator)
    {
      return node->data_.second;
    }
    else
    {
      throw std::invalid_argument("Key already exists");
    }
  }

  balanceNode(node);
  return *result;
}

template< class Key, class Value, class Compare >
Value& novikov::AVLTree< Key, Value, Compare >::operator[](const Key& k)
{
  Value& res = insertNode(fakeroot_->left_, k, Value(), true, fakeroot_);
  updateHeight(fakeroot_);
  return res;
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::clear(Node* root)
{
  if (!root)
  {
    return;
  }
  clear(root->left_);
  clear(root->right_);
  delete root;
}

template< class Key, class Value, class Compare >
typename novikov::AVLTree< Key, Value, Compare >::Node*
novikov::AVLTree< Key, Value, Compare >::copyNodes(Node* other, Node* parent)
{
  if (!other)
  {
    return nullptr;
  }
  Node* newAVL = new Node;
  newAVL->data_ = other->data_;
  newAVL->height_ = other->height_;
  newAVL->parent_ = parent;
  newAVL->left_ = copyNodes(other->left_, newAVL);
  newAVL->right_ = copyNodes(other->right_, newAVL);
  return newAVL;
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::swap(AVLTree& other) noexcept
{
  std::swap(fakeroot_, other.fakeroot_);
  std::swap(compare_, other.compare_);
  std::swap(name_, other.name_);
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::push(Key k, Value v)
{
  insertNode(fakeroot_->left_, k, v, false, fakeroot_);
  updateHeight(fakeroot_);
}

template< class Key, class Value, class Compare >
const Value& novikov::AVLTree< Key, Value, Compare >::get(const Key& k) const
{
  Node* curr = fakeroot_->left_;
  while (curr)
  {
    if (compare_(k, curr->data_.first))
    {
      curr = curr->left_;
    }
    else if (compare_(curr->data_.first, k))
    {
      curr = curr->right_;
    }
    else
    {
      return curr->data_.second;
    }
  }
  throw std::out_of_range("Key does not exist");
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::deleteNode(Node*& node, const Key& k)
{
  if (!node)
  {
    throw std::out_of_range("Key does not exist");
  }

  if (compare_(k, node->data_.first))
  {
    deleteNode(node->left_, k);
  }
  else if (compare_(node->data_.first, k))
  {
    deleteNode(node->right_, k);
  }
  else
  {
    if (!node->left_ || !node->right_)
    {
      Node* temp = node->left_ ? node->left_ : node->right_;
      if (!temp)
      {
        temp = node;
        node = nullptr;
      }
      else
      {
        temp->parent_ = node->parent_;
        Node* toDelete = node;
        node = temp;
        temp = toDelete;
      }
      delete temp;
    }
    else
    {
      Node* temp = fallLeft(node->right_);
      node->data_ = temp->data_;
      deleteNode(node->right_, temp->data_.first);
    }
  }

  if (node)
  {
    balanceNode(node);
  }
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::drop(const Key& k)
{
  deleteNode(fakeroot_->left_, k);
  updateHeight(fakeroot_);
}

template< class Key, class Value, class Compare >
bool novikov::AVLTree< Key, Value, Compare >::has(const Key& k) const
{
  Node* curr = fakeroot_->left_;
  while (curr)
  {
    if (compare_(k, curr->data_.first))
    {
      curr = curr->left_;
    }
    else if (compare_(curr->data_.first, k))
    {
      curr = curr->right_;
    }
    else
    {
      return true;
    }
  }
  return false;
}

template< class Key, class Value >
novikov::Node< Key, Value >* novikov::fallLeft(Node< Key, Value >* node)
{
  if (!node)
  {
    return node;
  }
  while (node->left_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::setName(std::string name)
{
  name_ = name;
}

template< class Key, class Value, class Compare >
std::string novikov::AVLTree< Key, Value, Compare >::getName()
{
  return name_;
}

template< class Key, class Value, class Compare >
void novikov::AVLTree< Key, Value, Compare >::updateHeight(Node* node)
{
  if (node)
  {
    size_t h1 = getHeight(node->left_);
    size_t h2 = getHeight(node->right_);
    node->height_ = 1 + (h1 > h2 ? h1 : h2);
  }
}

template< class Key, class Value, class Compare >
size_t novikov::AVLTree< Key, Value, Compare >::getHeight(const Node* node) const
{
  return node ? node->height_ : 0;
}

template< class Key, class Value, class Compare >
size_t novikov::AVLTree< Key, Value, Compare >::height() const
{
  return getHeight(fakeroot_->left_);
}

template< class Key, class Value, class Compare >
size_t novikov::AVLTree< Key, Value, Compare >::height(const_iterator it) const
{
  return getHeight(it.curr_);
}

template< class Key, class Value >
novikov::AVLIterator< Key, Value >::AVLIterator(Node* other):
    curr_(other)
{}

template< class Key, class Value >
bool novikov::AVLIterator< Key, Value >::operator==(const AVLIterator< Key, Value >& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool novikov::AVLIterator< Key, Value >::operator!=(const AVLIterator< Key, Value >& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
novikov::AVLIterator< Key, Value >& novikov::AVLIterator< Key, Value >::operator++()
{
  if (curr_->right_)
  {
    curr_ = curr_->right_;
    curr_ = fallLeft(curr_);
  }
  else
  {
    while (curr_->parent_ != nullptr && curr_ != curr_->parent_->left_)
    {
      curr_ = curr_->parent_;
    }
    curr_ = curr_->parent_;
  }
  return *this;
}

template< class Key, class Value >
std::pair< Key, Value >& novikov::AVLIterator< Key, Value >::operator*()
{
  return curr_->data_;
}

template< class Key, class Value >
novikov::AVLConstIterator< Key, Value >::AVLConstIterator(const Node* other):
    curr_(other)
{}

template< class Key, class Value >
bool novikov::AVLConstIterator< Key, Value >::operator==(const AVLConstIterator< Key, Value >& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool novikov::AVLConstIterator< Key, Value >::operator!=(const AVLConstIterator< Key, Value >& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
novikov::AVLConstIterator< Key, Value >& novikov::AVLConstIterator< Key, Value >::operator++()
{
  if (curr_->right_)
  {
    curr_ = curr_->right_;
    curr_ = fallLeft(curr_);
  }
  else
  {
    while (curr_->parent_ != nullptr && curr_ != curr_->parent_->left_)
    {
      curr_ = curr_->parent_;
    }
    curr_ = curr_->parent_;
  }
  return *this;
}

template< class Key, class Value, class Compare >
novikov::AVLIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::begin()
{
  if (!fakeroot_->left_)
  {
    return end();
  }
  Node* min = fallLeft(fakeroot_->left_);
  return AVLIterator< Key, Value >(min);
}

template< class Key, class Value, class Compare >
novikov::AVLConstIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::cbegin()
{
  if (!fakeroot_->left_)
  {
    return cend();
  }
  Node* min = fallLeft(fakeroot_->left_);
  return AVLConstIterator< Key, Value >(min);
}

template< class Key, class Value, class Compare >
novikov::AVLIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::end()
{
  return AVLIterator< Key, Value >(fakeroot_);
}

template< class Key, class Value, class Compare >
novikov::AVLConstIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::cend()
{
  return AVLConstIterator< Key, Value >(fakeroot_);
}

template< class Key, class Value >
const std::pair< Key, Value >& novikov::AVLConstIterator< Key, Value >::operator*()
{
  return curr_->data_;
}

template< class Key, class Value, class Compare >
novikov::AVLIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::rotateLeft(iterator it)
{
  if (!it.curr_ || !it.curr_->parent_)
  {
    throw std::out_of_range("");
  }
  Node* buf = it.curr_->parent_;
  it.curr_->parent_ = buf->parent_;
  if (it.curr_->parent_)
  {
    if (it.curr_->parent_->right_ == buf)
    {
      it.curr_->parent_->right_ = it.curr_;
    }
    else
    {
      it.curr_->parent_->left_ = it.curr_;
    }
  }
  buf->right_ = it.curr_->left_;
  if (buf->right_)
  {
    buf->right_->parent_ = buf;
  }
  it.curr_->left_ = buf;
  if (it.curr_->left_)
  {
    it.curr_->left_->parent_ = it.curr_;
  }
  updateHeight(buf);
  updateHeight(it.curr_);

  return AVLIterator< Key, Value >(buf);
}

template< class Key, class Value, class Compare >
novikov::AVLIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::rotateRight(iterator it)
{
  if (!it.curr_ || !it.curr_->parent_)
  {
    throw std::out_of_range("");
  }
  Node* buf = it.curr_->parent_;
  it.curr_->parent_ = buf->parent_;
  if (it.curr_->parent_)
  {
    if (it.curr_->parent_->left_ == buf)
    {
      it.curr_->parent_->left_ = it.curr_;
    }
    else
    {
      it.curr_->parent_->right_ = it.curr_;
    }
  }
  buf->left_ = it.curr_->right_;
  if (buf->left_)
  {
    buf->left_->parent_ = buf;
  }
  it.curr_->right_ = buf;
  it.curr_->right_->parent_ = it.curr_;
  updateHeight(buf);
  updateHeight(it.curr_);

  return AVLIterator< Key, Value >(buf);
}

template< class Key, class Value, class Compare >
novikov::AVLIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::rotateLargeLeft(iterator it)
{
  rotateRight(it);
  return rotateLeft(it);
}

template< class Key, class Value, class Compare >
novikov::AVLIterator< Key, Value > novikov::AVLTree< Key, Value, Compare >::rotateLargeRight(iterator it)
{
  rotateLeft(it);
  return rotateRight(it);
}

#endif
