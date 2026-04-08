#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iter.hpp"
#include "citer.hpp"
#include <iostream>

namespace khalikov
{
  template < class T > class LCIter;

  template < class T > class LIter;

  template < class T > class List
  {
  private:
    Node< T > *h;

  public:
    List();
    ~List();
    List< T > &operator=(const List< T > &h);
    List(const List< T > &other);
    List(List< T > &&other) noexcept;
    List< T > &operator=(List< T > &&h);
    LIter< T > begin();
    LCIter< T > cbegin() const;

    Node< T > *findLast(Node< T > *h);
    void swap(List< T > &other) noexcept;
    size_t size() const noexcept;
    void clear();
    bool isEmpty() const noexcept;
    void eraseAfter(LIter< T > pos);
    void insertAfter(LIter< T > pos, const T &value);
    void popFront() noexcept;
    void pushFront(const T &value);
    void erase(LIter< T > pos);
    void remove(const T &value);
    void sort();
    void uniqueSort();
    void reverse() noexcept;
    void print() const;
    void pushBack(const T &value);
    void popBack() noexcept;
  };
}

template < class T > khalikov::Node< T > *khalikov::List< T >::findLast(khalikov::Node< T > *h)
{
  if (!h) {
    return nullptr;
  }
  Node< T > *last = h;
  while (last->next != h) {
    last = last->next;
  }
  return last;
}

template < class T > size_t khalikov::List< T >::size() const noexcept
{
  if (!h) {
    return 0;
  }
  size_t k = 0;
  Node< T > *temp = h;
  do {
    k++;
    temp = temp->next;
  } while (temp != h);
  return k;
}

template < class T >
khalikov::List< T >::List():
  h(nullptr)
{}

template < class T > khalikov::List< T >::~List()
{
  clear();
}

template < class T >
khalikov::List< T >::List(const List< T > &other):
  h(nullptr)
{
  if (!other.h) {
    h = nullptr;
    return;
  }

  try {
    h = new Node< T >{other.h->val, nullptr};
    h->next = h;
    Node< T > *curr = h;
    Node< T > *curr_other = other.h->next;

    while (curr_other != other.h) {
      curr->next = new Node< T >{curr_other->val, h};
      curr = curr->next;
      curr_other = curr_other->next;
    }
    curr->next = h;
  } catch (...) {
    clear();
    throw;
  }
}

template < class T >
khalikov::List< T >::List(List< T > &&other) noexcept:
  h(other.h)
{
  other.h = nullptr;
}

template < class T > khalikov::List< T > &khalikov::List< T >::operator=(List< T > &&other)
{
  if (this == std::addressof(other)) {
    return *this;
  }
  List< T > cpy(std::move(other));
  swap(cpy);
  return *this;
}

template < class T > khalikov::List< T > &khalikov::List< T >::operator=(const List< T > &other)
{
  if (this != std::addressof(other)) {
    List< T > cpy(other);
    swap(cpy);
  }
  return *this;
}

template < class T > void khalikov::List< T >::swap(List< T > &other) noexcept
{
  std::swap(h, other.h);
}

template < class T > void khalikov::List< T >::clear()
{
  if (!h) {
    return;
  }
  Node< T > *curr = h->next;
  while (curr != h) {
    Node< T > *temp = curr;
    curr = curr->next;
    delete temp;
  }
  delete h;
  h = nullptr;
}

template < class T > khalikov::LIter< T > khalikov::List< T >::begin()
{
  return LIter< T >(h);
}

template < class T > khalikov::LCIter< T > khalikov::List< T >::cbegin() const
{
  return LCIter< T >(h);
}

template < class T > bool khalikov::List< T >::isEmpty() const noexcept
{
  return h == nullptr;
}

template < class T > void khalikov::List< T >::eraseAfter(LIter< T > pos)
{
  Node< T > *erase = pos.curr->next;
  pos.curr->next = erase->next;
  delete erase;
}

template < class T > void khalikov::List< T >::insertAfter(LIter< T > pos, const T &value)
{
  Node< T > *new_element = new Node< T >{value, pos.curr->next};
  pos.curr->next = new_element;
}

template < class T > void khalikov::List< T >::popFront() noexcept
{
  if (!h) {
    return;
  }
  if (h->next == h) {
    delete h;
    h = nullptr;
    return;
  }
  Node< T > *last = findLast(h);
  Node< T > *temp = h;
  h = h->next;
  last->next = h;
  delete temp;
}

template < class T > void khalikov::List< T >::pushFront(const T &value)
{
  if (!h) {
    h = new Node< T >{value, nullptr};
    h->next = h;
    return;
  }
  Node< T > *last = findLast(h);
  Node< T > *temp = new Node< T >{value, h};
  last->next = temp;
  h = temp;
}

template < class T > void khalikov::List< T >::erase(LIter< T > pos)
{
  if (pos.curr == h && h->next == h) {
    delete h;
    h = nullptr;
    return;
  }

  if (pos.curr == h) {
    Node< T > *last = findLast(h);
    Node< T > *temp = h;
    h = h->next;
    last->next = h;
    delete temp;
    return;
  }

  Node< T > *prev = h;
  while (prev->next != pos.curr) {
    prev = prev->next;
  }
  prev->next = pos.curr->next;
  delete pos.curr;
}

template < class T > void khalikov::List< T >::remove(const T &value)
{
  if (!h) {
    return;
  }
  Node< T > *curr = h;
  Node< T > *prev = findLast(h);
  size_t count = size();
  for (size_t i = 0; i < count; ++i) {
    if (curr->val == value) {
      if (curr == h) {
        if (h->next == h) {
          clear();
          return;
        }
        Node< T > *temp = h;
        h = h->next;
        prev->next = h;
        delete temp;
        curr = h;
      } else {
        prev->next = curr->next;
        delete curr;
        curr = prev->next;
      }
    } else {
      prev = curr;
      curr = curr->next;
    }
    if (!h) {
      return;
    }
  }
}

template < class T > void khalikov::List< T >::sort()
{
  if (!h || h->next == h) {
    return;
  }
  bool swapped;
  do {
    swapped = false;
    Node< T > *curr = h;
    while (curr->next != h) {
      if (curr->val > curr->next->val) {
        std::swap(curr->val, curr->next->val);
        swapped = true;
      }
      curr = curr->next;
    }
  } while (swapped);
}

template < class T > void khalikov::List< T >::uniqueSort()
{
  sort();
  Node< T > *curr = h;
  while (curr->next != h) {
    if (curr->val == curr->next->val) {
      Node< T > *temp = curr->next;
      curr->next = temp->next;
      delete temp;
    } else {
      curr = curr->next;
    }
  }
}

template < class T > void khalikov::List< T >::print() const
{
  if (!h) {
    return;
  }
  Node< T > *curr = h;
  do {
    std::cout << curr->val;
    if (curr->next != h) {
      std::cout << " ";
    }
    curr = curr->next;
  } while (curr != h);
}

template < class T > void khalikov::List< T >::reverse() noexcept
{
  if (!h || h->next == h) {
    return;
  }
  Node< T > *prev = nullptr;
  Node< T > *curr = h;
  Node< T > *next = nullptr;
  Node< T > *last = findLast(h);
  last->next = nullptr;
  while (curr != nullptr) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }
  h->next = prev;
  h = prev;
}

template < class T > void khalikov::List< T >::pushBack(const T &value)
{
  if (!h) {
    h = new Node< T >{value, nullptr};
    h->next = h;
    return;
  }
  Node< T > *newNode = new Node< T >{value, h};
  Node< T > *curr = h;
  while (curr->next != h) {
    curr = curr->next;
  }
  curr->next = newNode;
}

template < class T > void khalikov::List< T >::popBack() noexcept
{
  if (!h) {
    return;
  }
  if (h->next == h) {
    clear();
  }
  Node< T > *curr = h;
  Node< T > *prev = nullptr;
  while (curr->next != h) {
    prev = curr;
    curr = curr->next;
  }
  prev->next = h;
  delete curr;
}

#endif
