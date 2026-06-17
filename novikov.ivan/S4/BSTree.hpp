#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace novikov
{
  template< class Key, class Value, class Compare >
  class BSTree;
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
  class BSTIterator {
    using Node = novikov::Node< Key, Value >;

  public:
    explicit BSTIterator(Node* other);
    bool operator==(const BSTIterator< Key, Value >& other) const;
    bool operator!=(const BSTIterator< Key, Value >& other) const;
    BSTIterator< Key, Value >& operator++();
    std::pair< Key, Value >& operator*();

  private:
    Node* curr_;
    template< class K, class V, class C >
    friend class BSTree;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
    using Node = novikov::Node< Key, Value >;

  public:
    explicit BSTConstIterator(const Node* other);
    bool operator==(const BSTConstIterator< Key, Value >& other) const;
    bool operator!=(const BSTConstIterator< Key, Value >& other) const;
    BSTConstIterator< Key, Value >& operator++();
    const std::pair< Key, Value >& operator*();

  private:
    const Node* curr_;
    template< class K, class V, class C >
    friend class BSTree;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
    using Node = novikov::Node< Key, Value >;
    friend class BSTIterator< Key, Value >;
    friend class BSTConstIterator< Key, Value >;

  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    BSTree& operator=(BSTree other);
    Value& operator[](const Key& k);

    void push(Key k, Value v);
    const Value& get(const Key& k) const;
    void drop(const Key& k);
    bool has(const Key& k) const;

    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;
    iterator rotateLeft(iterator it);
    iterator rotateRight(iterator it);
    iterator rotateLargeLeft(iterator it);
    iterator rotateLargeRight(iterator it);

    size_t height() const;
    size_t height(const_iterator it) const;
    void setName(std::string name);
    std::string getName();
    BSTIterator< Key, Value > begin();
    BSTConstIterator< Key, Value > cbegin();
    BSTIterator< Key, Value > end();
    BSTConstIterator< Key, Value > cend();

  private:
    Node* fakeroot_;
    Compare compare_;
    std::string name_;

    void clear(Node* fakeroot);
    Node* copyNodes(Node* other, Node* parent);
    void swap(BSTree& other) noexcept;
    Value& insertNode(Node*& node, Key k, Value v, bool isOperator, Node* parent = nullptr);
    void updateHeight(Node* node);
    size_t getHeight(const Node* node) const;
  };

  template< class Key, class Value >
  Node< Key, Value >* fallLeft(Node< Key, Value >* node);
}

template< class Key, class Value, class Compare >
novikov::BSTree< Key, Value, Compare >::BSTree():
    fakeroot_(new Node{{Key(), Value()}}),
    compare_(Compare())
{}

template< class Key, class Value, class Compare >
novikov::BSTree< Key, Value, Compare >::~BSTree()
{
  clear(fakeroot_);
}

template< class Key, class Value, class Compare >
novikov::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
    fakeroot_(new Node{{Key(), Value()}}),
    compare_(other.compare_),
    name_(other.name_)
{
  fakeroot_->left_ = copyNodes(other.fakeroot_->left_, fakeroot_);
}

template< class Key, class Value, class Compare >
novikov::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
    fakeroot_(other.fakeroot_),
    compare_(std::move(other.compare_)),
    name_(std::move(other.name_))
{
  other.fakeroot_ = nullptr;
}

template< class Key, class Value, class Compare >
novikov::BSTree< Key, Value, Compare >& novikov::BSTree< Key, Value, Compare >::operator=(BSTree other)
{
  swap(other);
  return *this;
}

template< class Key, class Value, class Compare >
Value& novikov::BSTree< Key, Value, Compare >::insertNode(Node*& node, Key k, Value v, bool isOperator, Node* parent)
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
    node->left_->parent_ = node;
  }
  else if (compare_(node->data_.first, k))
  {
    result = &insertNode(node->right_, k, v, isOperator, node);
    node->right_->parent_ = node;
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

  updateHeight(node);
  return *result;
}

template< class Key, class Value, class Compare >
Value& novikov::BSTree< Key, Value, Compare >::operator[](const Key& k)
{
  Value& res = insertNode(fakeroot_->left_, k, Value(), true, fakeroot_);
  updateHeight(fakeroot_);
  return res;
}

template< class Key, class Value, class Compare >
void novikov::BSTree< Key, Value, Compare >::clear(Node* root)
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
typename novikov::BSTree< Key, Value, Compare >::Node*
    novikov::BSTree< Key, Value, Compare >::copyNodes(Node* other, Node* parent)
{
  if (!other)
  {
    return nullptr;
  }
  Node* newBST = new Node;
  newBST->data_ = other->data_;
  newBST->height_ = other->height_;
  newBST->parent_ = parent;
  newBST->left_ = copyNodes(other->left_, newBST);
  newBST->right_ = copyNodes(other->right_, newBST);
  return newBST;
}

template< class Key, class Value, class Compare >
void novikov::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(fakeroot_, other.fakeroot_);
  std::swap(compare_, other.compare_);
  std::swap(name_, other.name_);
}

template< class Key, class Value, class Compare >
void novikov::BSTree< Key, Value, Compare >::push(Key k, Value v)
{
  insertNode(fakeroot_->left_, k, v, false, fakeroot_);
  updateHeight(fakeroot_);
}

template< class Key, class Value, class Compare >
const Value& novikov::BSTree< Key, Value, Compare >::get(const Key& k) const
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
void novikov::BSTree< Key, Value, Compare >::drop(const Key& k)
{
  Node* parent = fakeroot_;
  Node* curr = fakeroot_->left_;

  while (curr)
  {
    if (compare_(k, curr->data_.first))
    {
      parent = curr;
      curr = curr->left_;
    }
    else if (compare_(curr->data_.first, k))
    {
      parent = curr;
      curr = curr->right_;
    }
    else
      break;
  }

  if (!curr)
    throw std::out_of_range("Key does not exist");

  if (curr->left_ && curr->right_)
  {
    Node* newCurr = fallLeft(curr->right_);
    Node* newCurrParent = curr;
    if (newCurrParent->right_ != newCurr)
    {
      newCurrParent = curr->right_;
      while (newCurrParent->left_ != newCurr)
      {
        newCurrParent = newCurrParent->left_;
      }
    }

    curr->data_ = newCurr->data_;
    parent = newCurrParent;
    curr = newCurr;
  }

  Node* child;
  if (curr->left_)
  {
    child = curr->left_;
  }
  else
  {
    child = curr->right_;
  }

  if (parent->left_ == curr)
  {
    parent->left_ = child;
  }
  else
  {
    parent->right_ = child;
  }

  if (child)
  {
    child->parent_ = parent;
  }

  delete curr;
}

template< class Key, class Value, class Compare >
bool novikov::BSTree< Key, Value, Compare >::has(const Key& k) const
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
void novikov::BSTree< Key, Value, Compare >::setName(std::string name)
{
  name_ = name;
}

template< class Key, class Value, class Compare >
std::string novikov::BSTree< Key, Value, Compare >::getName()
{
  return name_;
}

template< class Key, class Value, class Compare >
void novikov::BSTree< Key, Value, Compare >::updateHeight(Node* node)
{
  if (node)
  {
    size_t h1 = getHeight(node->left_);
    size_t h2 = getHeight(node->right_);
    if (h1 > h2)
    {
      node->height_ = 1 + h1;
    }
    else
    {
      node->height_ = 1 + h2;
    }
  }
}

template< class Key, class Value, class Compare >
size_t novikov::BSTree< Key, Value, Compare >::getHeight(const Node* node) const
{
  if (node)
  {
    return node->height_;
  }
  else
  {
    return 0;
  }
}

template< class Key, class Value, class Compare >
size_t novikov::BSTree< Key, Value, Compare >::height() const
{
  return getHeight(fakeroot_->left_);
}

template< class Key, class Value, class Compare >
size_t novikov::BSTree< Key, Value, Compare >::height(const_iterator it) const
{
  return getHeight(it.curr_);
}

template< class Key, class Value >
novikov::BSTIterator< Key, Value >::BSTIterator(Node* other):
  curr_(other)
{}

template< class Key, class Value >
bool novikov::BSTIterator< Key, Value >::operator==(const BSTIterator< Key, Value >& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool novikov::BSTIterator< Key, Value >::operator!=(const BSTIterator< Key, Value >& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
novikov::BSTIterator< Key, Value >& novikov::BSTIterator< Key, Value >::operator++()
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
std::pair< Key, Value >& novikov::BSTIterator< Key, Value >::operator*()
{
  return curr_->data_;
}

template< class Key, class Value >
novikov::BSTConstIterator< Key, Value >::BSTConstIterator(const Node* other):
  curr_(other)
{}

template< class Key, class Value >
bool novikov::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator< Key, Value >& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool novikov::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator< Key, Value >& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
novikov::BSTConstIterator< Key, Value >& novikov::BSTConstIterator< Key, Value >::operator++()
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
novikov::BSTIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::begin()
{
  if (!fakeroot_->left_)
  {
    return end();
  }
  Node* min = fallLeft(fakeroot_->left_);
  return BSTIterator< Key, Value >(min);
}

template< class Key, class Value, class Compare >
novikov::BSTConstIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::cbegin()
{
  if (!fakeroot_->left_)
  {
    return cend();
  }
  Node* min = fallLeft(fakeroot_->left_);
  return BSTConstIterator< Key, Value >(min);
}

template< class Key, class Value, class Compare >
novikov::BSTIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::end()
{
  return BSTIterator< Key, Value >(fakeroot_);
}

template< class Key, class Value, class Compare >
novikov::BSTConstIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::cend()
{
  return BSTConstIterator< Key, Value >(fakeroot_);
}

template< class Key, class Value >
const std::pair< Key, Value >& novikov::BSTConstIterator< Key, Value >::operator*()
{
  return curr_->data_;
}

template< class Key, class Value, class Compare >
novikov::BSTIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::rotateLeft(iterator it)
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

  return BSTIterator< Key, Value >(buf);
}

template< class Key, class Value, class Compare >
novikov::BSTIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::rotateRight(iterator it)
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

  return BSTIterator< Key, Value >(buf);
}

template< class Key, class Value, class Compare >
novikov::BSTIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::rotateLargeLeft(iterator it)
{
  rotateRight(it);
  return rotateLeft(it);
}

template< class Key, class Value, class Compare >
novikov::BSTIterator< Key, Value > novikov::BSTree< Key, Value, Compare >::rotateLargeRight(iterator it)
{
  rotateLeft(it);
  return rotateRight(it);
}

#endif
