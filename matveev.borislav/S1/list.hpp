#ifndef MATVEEV_LIST_HPP
#define MATVEEV_LIST_HPP

#include <cstddef>

namespace matveev
{

template< class T >
struct Node
{
  T data;
  Node* next;

  Node()
    : data(),
      next(nullptr)
  {
  }

  Node(const T& value)
    : data(value),
      next(nullptr)
  {
  }
};

template< class T >
class LIter
{
public:
  LIter()
    : node_(nullptr)
  {
  }

  T& operator*() const
  {
    return node_->data;
  }

  T* operator->() const
  {
    return &(node_->data);
  }

  LIter& operator++()
  {
    node_ = node_->next;
    return *this;
  }

  bool operator==(const LIter& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const LIter& other) const
  {
    return node_ != other.node_;
  }

private:
  Node< T >* node_;

  template< class >
  friend class List;
};

template< class T >
class LCIter
{
public:
  LCIter()
    : node_(nullptr)
  {
  }

  const T& operator*() const
  {
    return node_->data;
  }

  const T* operator->() const
  {
    return &(node_->data);
  }

  LCIter& operator++()
  {
    node_ = node_->next;
    return *this;
  }

  bool operator==(const LCIter& other) const
  {
   return node_ == other.node_;
  }

  bool operator!=(const LCIter& other) const
  {
    return node_ != other.node_;
  }

private:
  const Node< T >* node_;

  template< class >
  friend class List;
};

template< class T >
class List
{
public:
  List()
  {
    sentinel_ = new Node< T >();
    sentinel_->next = nullptr;
  }
  List(const List& other)
  {
    sentinel_ = new Node<T>();
    sentinel_->next = nullptr;

    Node<T>* tail = sentinel_;
    Node<T>* cur = other.sentinel_->next;

    while (cur != nullptr)
    {
      Node<T>* node = new Node<T>(cur->data);
      tail->next = node;
      tail = node;
      cur = cur->next;
    }
  }

  List& operator=(const List& other)
  {
    if (this == &other)
    {
      return *this;
    }

    clear();

    Node<T>* tail = sentinel_;
    Node<T>* cur = other.sentinel_->next;

    while (cur != nullptr)
    {
      Node<T>* node = new Node<T>(cur->data);
      tail->next = node;
      tail = node;
      cur = cur->next;
    }

    return *this;
  }

  ~List()
  {
    clear();
    delete sentinel_;
  }

  void clear()
  {
    Node< T >* cur = sentinel_->next;

    while (cur != nullptr)
    {
      Node< T >* tmp = cur;
      cur = cur->next;
      delete tmp;
    }

    sentinel_->next = nullptr;
  }

  void removeFront()
  {
    Node< T >* first = sentinel_->next;

    if (first == nullptr)
    {
      return;
    }

    sentinel_->next = first->next;
    delete first;
  }

  LIter< T > beforeBegin()
  {
    LIter< T > it;
    it.node_ = sentinel_;
    return it;
  }

  LCIter< T > beforeBegin() const
  {
    LCIter< T > it;
    it.node_ = sentinel_;
    return it;
  }

  LIter< T > begin()
  {
    LIter< T > it;
    it.node_ = sentinel_->next;
    return it;
  }

  LCIter< T > begin() const
  {
    LCIter< T > it;
    it.node_ = sentinel_->next;
    return it;
  }

  LIter< T > end()
  {
    LIter< T > it;
    it.node_ = nullptr;
    return it;
  }

  LCIter< T > end() const
  {
    LCIter< T > it;
    it.node_ = nullptr;
    return it;
  }

  LIter< T > insertAfter(LIter< T > pos, const T& value)
  {
    Node< T >* node = pos.node_;

    Node< T >* newNode = new Node< T >(value);
    newNode->next = node->next;
    node->next = newNode;

    LIter< T > it;
    it.node_ = newNode;

    return it;
  }

  void eraseAfter(LIter< T > pos)
  {
    Node< T >* node = pos.node_;
    Node< T >* target = node->next;

    if (target == nullptr)
    {
      return;
    }

    node->next = target->next;
    delete target;
  }

private:
  Node< T >* sentinel_;
};

}

#endif
