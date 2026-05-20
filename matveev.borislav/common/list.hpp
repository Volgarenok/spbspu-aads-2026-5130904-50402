#ifndef MATVEEV_LIST_HPP
#define MATVEEV_LIST_HPP

#include <cstddef>
#include <utility>
#include <functional>

namespace matveev
{

template< class T >
struct Node
{
  T data;
  Node< T >* next;

  Node():
    data(),
    next(nullptr)
  {}

  Node(const T& value):
    data(value),
    next(nullptr)
  {}
};

template< class T >
class LIter
{
public:
  LIter():
    node_(nullptr)
  {}

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
  LCIter():
    node_(nullptr)
  {}

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
    sentinel_ = new Node< T >();
    sentinel_->next = nullptr;

    Node< T >* tail = sentinel_;
    Node< T >* cur = other.sentinel_->next;

    while (cur != nullptr)
    {
      Node< T >* node = new Node< T >(cur->data);
      tail->next = node;
      tail = node;
      cur = cur->next;
    }
  }

  List(List&& other)
  {
    sentinel_ = other.sentinel_;
    other.sentinel_ = new Node< T >();
    other.sentinel_->next = nullptr;
  }

  ~List()
  {
    clear();
    delete sentinel_;
  }

  List& operator=(const List& other)
  {
    if (this != &other)
    {
      List temp(other);
      swap(temp);
    }

    return *this;
  }

  List& operator=(List&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    clear();
    delete sentinel_;

    sentinel_ = other.sentinel_;
    other.sentinel_ = new Node< T >();
    other.sentinel_->next = nullptr;

    return *this;
  }

  void swap(List& other) noexcept
  {
    std::swap(sentinel_, other.sentinel_);
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
    if (pos.node_ == nullptr)
    {
      return end();
    }

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

    if (node == nullptr)
    {
      return;
    }

    Node< T >* target = node->next;

    if (target == nullptr)
    {
      return;
    }

    node->next = target->next;
    delete target;
  }

  void spliceAfter(LIter< T > pos, List& other, LIter< T > before)
  {
    (void) other;

    if (pos.node_ == nullptr || before.node_ == nullptr || before.node_->next == nullptr)
    {
      return;
    }

    Node< T >* moved = before.node_->next;
    before.node_->next = moved->next;

    moved->next = pos.node_->next;
    pos.node_->next = moved;
  }

  void spliceAfter(LIter< T > pos, List& other, LIter< T > beforeFirst, LIter< T > beforeLast)
  {
    (void) other;

    if (pos.node_ == nullptr || beforeFirst.node_ == nullptr || beforeFirst.node_ == beforeLast.node_)
    {
      return;
    }

    Node< T >* first = beforeFirst.node_->next;

    if (first == beforeLast.node_)
    {
      return;
    }

    Node< T >* last = first;

    while (last->next != beforeLast.node_)
    {
      last = last->next;
    }

    beforeFirst.node_->next = beforeLast.node_;

    last->next = pos.node_->next;
    pos.node_->next = first;
  }

  template< class Compare >
  void merge(List& other, Compare comp)
  {
    if (this == &other)
    {
      return;
    }

    Node< T >* thisPrev = sentinel_;
    Node< T >* otherPrev = other.sentinel_;

    while (thisPrev->next != nullptr && otherPrev->next != nullptr)
    {
      if (comp(otherPrev->next->data, thisPrev->next->data))
      {
        Node< T >* moved = otherPrev->next;
        otherPrev->next = moved->next;

        moved->next = thisPrev->next;
        thisPrev->next = moved;
      }

      thisPrev = thisPrev->next;
    }

    if (otherPrev->next != nullptr)
    {
      thisPrev->next = otherPrev->next;
      otherPrev->next = nullptr;
    }
  }

  void merge(List& other)
  {
    merge(other, std::less< T >());
  }

  template< class Compare >
  void sort(Compare comp)
  {
    if (sentinel_->next == nullptr || sentinel_->next->next == nullptr)
    {
      return;
    }

    Node< T > sorted;
    sorted.next = nullptr;

    while (sentinel_->next != nullptr)
    {
      Node< T >* moved = sentinel_->next;
      sentinel_->next = moved->next;

      Node< T >* prev = &sorted;

      while (prev->next != nullptr && !comp(moved->data, prev->next->data))
      {
        prev = prev->next;
      }

      moved->next = prev->next;
      prev->next = moved;
    }

    sentinel_->next = sorted.next;
  }

void sort()
{
  sort(std::less< T >());
}
private:
  Node< T >* sentinel_;
};

}

#endif
