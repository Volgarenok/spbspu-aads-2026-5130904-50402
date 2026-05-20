#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <List.hpp>
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace
{
  template <class Key, class Value>
  struct Node
  {
    std::pair<Key, Value> data_;
    Node *left_ = nullptr;
    Node *right_ = nullptr;
    size_t height_;
    Node *parent_;
  };
}

namespace lavrentev
{
  template< class Key, class Value, class Compare >
  struct BSTree;
}

using BSTList = lavrentev::List<lavrentev::BSTree<size_t, std::string, std::less<size_t>>>;
using cmd_t = void (*)(std::istream &in, std::ostream &out, BSTList &bstl);

namespace lavrentev
{
  template< class Key, class Value >
  class BSTIterator {
    using Node = Node<Key, Value>;
  public:
    explicit BSTIterator(Node *other);

    bool operator==(const BSTIterator<Key, Value > &other) const;
    bool operator!=(const BSTIterator<Key, Value > &other) const;
    BSTIterator<Key, Value > &operator++();
    std::pair<Key, Value> &operator*();

  private:
    Node *curr_;
  };

  template< class Key, class Value >
  class BSTConstIterator {
    using Node = Node<Key, Value>;
  public:
    explicit BSTConstIterator(const Node *other);

    bool operator==(const BSTConstIterator<Key, Value > &other) const;
    bool operator!=(const BSTConstIterator<Key, Value > &other) const;
    BSTConstIterator<Key, Value > &operator++();
    const std::pair<Key, Value> &operator*();

  private:
    const Node *curr_;
  };

  template< class Key, class Value, class Compare >
  struct BSTree {
    using Node = Node<Key, Value>;
    friend class BSTIterator<Key, Value>;
    friend class BSTConstIterator<Key, Value>;
  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other) noexcept;
    BSTree &operator=(BSTree other);
    Value &operator[](const Key &k);

    void push(Key k, Value v);
    const Value &get(const Key &k) const;
    void drop(const Key &k);
    bool has(const Key &k) const;

    using const_iterator = BSTConstIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it); //realize
    const_iterator rotateRight(const_iterator it); //realize
    const_iterator rotateLargeLeft(const_iterator it); //realize
    const_iterator rotateLargeRight(const_iterator it); //realize

    size_t height(const_iterator it) const;
    size_t height() const;
    void setName(std::string name);
    std::string getName();
    BSTIterator<Key, Value> begin();
    BSTConstIterator<Key, Value> cbegin();
    BSTIterator<Key, Value> end();
    BSTConstIterator<Key, Value> cend();


  private:
    Node *fakeroot_;
    Compare compare_;
    std::string name_;

    void clear(Node *fakeroot);
    Node *copyNodes(Node *other);
    void swap(BSTree &other) noexcept;
    Value &insertNode(Node* &node, Key k, Value v, bool isOperator);
    void updateHeight(Node *node);
    size_t getHeight(Node *node);
  };

  template< class Key, class Value >
  Node<Key, Value> *fallLeft(Node<Key, Value> *node);

  void print(std::istream &in, std::ostream &out, BSTList &bstl);
  void complement(std::istream &in, std::ostream &out, BSTList &bstl);
  void intersect(std::istream &in, std::ostream &out, BSTList &bstl);
  void unionn(std::istream &in, std::ostream &out, BSTList &bstl);
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::BSTree():
  fakeroot_(new Node{{Key(), Value()}}),
  compare_(Compare())
{}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::~BSTree()
{
  clear(fakeroot_);
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::BSTree(const BSTree &other)
{
  fakeroot_ = new Node();
  fakeroot_->left_ = copyNodes(other.fakeroot_->left_);
  compare_ = other.compare_;
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare>::BSTree(BSTree &&other) noexcept:
  BSTree()
{
  swap(other);
}

template< class Key, class Value, class Compare >
lavrentev::BSTree<Key, Value, Compare> &lavrentev::BSTree<Key, Value, Compare>::operator=(BSTree other)
{
  swap(other);
  return *this;
}

template< class Key, class Value, class Compare >
Value &lavrentev::BSTree<Key, Value, Compare>::insertNode(Node* &node, Key k, Value v, bool isOperator)
{
  if (node == nullptr)
  {
    node = new Node();
    node->data_ = {k, v};
    node->height_ = 1;
    return node->data_.second;
  }

  Value* result = nullptr;

  if (compare_(k, node->data_.first))
  {
    result = &insertNode(node->left_, k, v, isOperator);
  }
  else if (compare_(node->data_.first, k))
  {
    result = &insertNode(node->right_, k, v, isOperator);
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
Value &lavrentev::BSTree<Key, Value, Compare>::operator[](const Key &k)
{
  Value &res = insertNode(fakeroot_->left_, k, Value(), true);
  updateHeight(fakeroot_);
  return res;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::clear(Node *root)
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
typename lavrentev::BSTree<Key, Value, Compare>::Node
  *lavrentev::BSTree<Key, Value, Compare>::copyNodes(Node *other)
{
  if (!other)
  {
    return nullptr;
  }
  Node* newBST = new Node;
  newBST->data_ = other->data_;
  newBST->left_ = copyNodes(other->left_);
  newBST->right_ = copyNodes(other->right_);
  return newBST;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::swap(BSTree &other) noexcept
{
  std::swap(fakeroot_, other.fakeroot_);
  std::swap(compare_, other.compare_);
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::push(Key k, Value v)
{
  insertNode(fakeroot_->left_, k, v, false);
  updateHeight(fakeroot_);
}

template< class Key, class Value, class Compare >
const Value &lavrentev::BSTree<Key, Value, Compare>::get(const Key &k) const
{
  Node *curr = fakeroot_->left_;
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
void lavrentev::BSTree<Key, Value, Compare>::drop(const Key &k)
{
  Node *parent = fakeroot_;
  Node *curr = fakeroot_->left_;

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
    else break;
  }

  if (!curr) throw std::out_of_range("Key does not exist");

  if (curr->left_ && curr->right_)
  {
    Node *newCurr = fallLeft(curr->right_);
    Node *newCurrParent = curr;
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

  Node *child;
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

  delete curr;
}

template< class Key, class Value >
Node<Key, Value>
  *lavrentev::fallLeft(Node<Key, Value> *node)
{
  if (!node)
  {
    return node;
  }
  while(node->left_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::setName(std::string name)
{
  name_ = name;
}

template< class Key, class Value, class Compare >
std::string lavrentev::BSTree<Key, Value, Compare>::getName()
{
  return name_;
}

template< class Key, class Value, class Compare >
void lavrentev::BSTree<Key, Value, Compare>::updateHeight(Node *node)
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
size_t lavrentev::BSTree<Key, Value, Compare>::getHeight(Node *node)
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
size_t lavrentev::BSTree<Key, Value, Compare>::height() const
{
  return getHeight(fakeroot_->left_);
}

template< class Key, class Value, class Compare >
size_t lavrentev::BSTree<Key, Value, Compare>::height(const_iterator it) const
{
  return getHeight(*it);
}

template< class Key, class Value >
lavrentev::BSTIterator<Key, Value>::BSTIterator(Node *other):
  curr_(other)
{
}

template< class Key, class Value >
bool lavrentev::BSTIterator<Key, Value>::operator==(const BSTIterator<Key, Value > &other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool lavrentev::BSTIterator<Key, Value>::operator!=(const BSTIterator<Key, Value > &other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
lavrentev::BSTIterator<Key, Value> &lavrentev::BSTIterator<Key, Value>::operator++()
{
  if (curr_->right_)
  {
    curr_ = curr_->right_;
    curr_ = fallLeft(curr_);
  }
  else
  {
    while(curr_->parent_ != nullptr && curr_ != curr_->parent_->left_)
    {
      curr_ = curr_->parent_;
    }
    curr_ = curr_->parent_;
  }
  return *this;
}

template< class Key, class Value >
std::pair<Key, Value> &lavrentev::BSTIterator<Key, Value>::operator*() 
{
  return curr_->data_;
}

template< class Key, class Value >
lavrentev::BSTConstIterator<Key, Value>::BSTConstIterator(const Node *other):
  curr_(other)
{
}

template< class Key, class Value >
bool lavrentev::BSTConstIterator<Key, Value>::operator==(const BSTConstIterator<Key, Value > &other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool lavrentev::BSTConstIterator<Key, Value>::operator!=(const BSTConstIterator<Key, Value > &other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
lavrentev::BSTConstIterator<Key, Value> &lavrentev::BSTConstIterator<Key, Value>::operator++()
{
  if (curr_->right_)
  {
    curr_ = curr_->right_;
    curr_ = fallLeft(curr_);
  }
  else
  {
    while(curr_->parent_ != nullptr && curr_ != curr_->parent_->left_)
    {
      curr_ = curr_->parent_;
    }
    curr_ = curr_->parent_;
  }
  return *this;
}

template< class Key, class Value, class Compare >
lavrentev::BSTIterator<Key, Value> lavrentev::BSTree<Key, Value, Compare>::begin()
{
  Node *min = fallLeft(fakeroot_->left_);
  return BSTIterator<Key, Value>(min);
}

template< class Key, class Value, class Compare >
lavrentev::BSTConstIterator<Key, Value> lavrentev::BSTree<Key, Value, Compare>::cbegin()
{
  Node *min = fallLeft(fakeroot_->left_);
  return BSTConstIterator<Key, Value>(min);
}

template< class Key, class Value, class Compare >
lavrentev::BSTIterator<Key, Value> lavrentev::BSTree<Key, Value, Compare>::end()
{
  Node *min = fallLeft(fakeroot_->left_);
  return BSTIterator<Key, Value>(min);
}

template< class Key, class Value, class Compare >
lavrentev::BSTConstIterator<Key, Value> lavrentev::BSTree<Key, Value, Compare>::cend()
{
  Node *min = fallLeft(fakeroot_->left_);
  return BSTConstIterator<Key, Value>(min);
}

template< class Key, class Value >
const std::pair<Key, Value> &lavrentev::BSTConstIterator<Key, Value>::operator*() 
{
  return *curr_->data_;
}

inline void lavrentev::print(std::istream &in, std::ostream &out, BSTList &bstl)
{
  std::string name;
  in >> name;
  LIter<BSTree<size_t, std::string, std::less<size_t>>> it = bstl.begin();
  while (it != nullptr)
  {
    if ((*it).getName() == name)
    {
      break;
    }
    ++it;
  }
  if (it == nullptr)
  {
    throw std::out_of_range("");
  }
  BSTIterator<size_t, std::string> bstIt = (*it).begin();
  if (bstIt == (*it).end())
  {
    out << "<EMPTY>" << "\n";
    return;
  }
  out << name << " " << (*bstIt).first << " " << (*bstIt).second;
  ++bstIt;
  for(; bstIt != (*it).end(); ++bstIt)
  {
    out << " " << (*bstIt).first << " " << (*bstIt).second;
  }
  out << "\n";
}

inline void lavrentev::complement(std::istream &in, std::ostream &out, BSTList &bstl)
{
  std::string newBstName, BstName1, BstName2;
  in >> newBstName >> BstName1 >> BstName2;
  BSTree<size_t, std::string, std::less<size_t>> newBst;
  newBst.setName(newBstName);
  LIter<BSTree<size_t, std::string, std::less<size_t>>> it = bstl.begin();
  BSTree<size_t, std::string, std::less<size_t>> *bst1 = nullptr;
  BSTree<size_t, std::string, std::less<size_t>> *bst2 = nullptr;
  bool is1 = false, is2 = false;
  while (it != nullptr)
  {
    if ((*it).getName() == BstName1)
    {
      is1 = true;
      bst1 = &(*it);
    }
    if ((*it).getName() == BstName2)
    {
      is2 = true;
      bst2 = &(*it);
    }
    if (is1 && is2)
    {
      break;
    }
    ++it;
  }
  if (!is1 || !is2)
  {
    throw std::out_of_range("");
  }
  BSTIterator<size_t, std::string> bstIt = (*bst1).begin();
  if (bstIt == (*bst1).end())
  {
    bstl.pushFront(std::move(newBst));
    return;
  }
  for(; bstIt != (*bst1).end(); ++bstIt)
  {
    if (!(*bst2).has((*bstIt).first))
    {
      newBst[(*bstIt).first] = (*bstIt).second;
    }
  }
  bstl.pushFront(std::move(newBst));
}

inline void lavrentev::intersect(std::istream &in, std::ostream &out, BSTList &bstl)
{
  std::string newBstName, BstName1, BstName2;
  in >> newBstName >> BstName1 >> BstName2;
  BSTree<size_t, std::string, std::less<size_t>> newBst;
  newBst.setName(newBstName);
  LIter<BSTree<size_t, std::string, std::less<size_t>>> it = bstl.begin();
  BSTree<size_t, std::string, std::less<size_t>> *bst1 = nullptr;
  BSTree<size_t, std::string, std::less<size_t>> *bst2 = nullptr;
  bool is1 = false, is2 = false;
  while (it != nullptr)
  {
    if ((*it).getName() == BstName1)
    {
      is1 = true;
      bst1 = &(*it);
    }
    if ((*it).getName() == BstName2)
    {
      is2 = true;
      bst2 = &(*it);
    }
    if (is1 && is2)
    {
      break;
    }
    ++it;
  }
  if (!is1 || !is2)
  {
    throw std::out_of_range("");
  }
  BSTIterator<size_t, std::string> bstIt = (*bst1).begin();
  if (bstIt == (*bst1).end())
  {
    bstl.pushFront(std::move(newBst));
    return;
  }
  for(; bstIt != (*bst1).end(); ++bstIt)
  {
    if ((*bst2).has((*bstIt).first))
    {
      newBst[(*bstIt).first] = (*bstIt).second;
    }
  }
  bstl.pushFront(std::move(newBst));
}

inline void lavrentev::unionn(std::istream &in, std::ostream &out, BSTList &bstl)
{
  std::string newBstName, BstName1, BstName2;
  in >> newBstName >> BstName1 >> BstName2;
  BSTree<size_t, std::string, std::less<size_t>> newBst;
  LIter<BSTree<size_t, std::string, std::less<size_t>>> it = bstl.begin();
  BSTree<size_t, std::string, std::less<size_t>> *bst1 = nullptr;
  BSTree<size_t, std::string, std::less<size_t>> *bst2 = nullptr;
  bool is1 = false, is2 = false;
  while (it != nullptr)
  {
    if ((*it).getName() == BstName1)
    {
      is1 = true;
      bst1 = &(*it);
    }
    if ((*it).getName() == BstName2)
    {
      is2 = true;
      bst2 = &(*it);
    }
    if (is1 && is2)
    {
      break;
    }
    ++it;
  }
  if (!is1 || !is2)
  {
    throw std::out_of_range("");
  }
  newBst = *bst1;
  newBst.setName(newBstName);
  BSTIterator<size_t, std::string> bstIt = (*bst2).begin();
  for(; bstIt != (*bst2).end(); ++bstIt)
  {
    if (!newBst.has((*bstIt).first))
    {
      newBst[(*bstIt).first] = (*bstIt).second;
    }
  }
  bstl.pushFront(std::move(newBst));
}

#endif
