#ifndef LIST_HPP
#define LIST_HPP
#include <algorithm>
#include <stdexcept>

namespace alisov
{
  template < class T >
  class BiList;

  template < class T >
  class BLIter
  {
  public:
    explicit BLIter(typename BiList< T >::Node *);
    bool operator==(const BLIter< T > &) const;
    bool operator!=(const BLIter< T > &) const;
    BLIter< T > &operator++();
    BLIter< T > &operator--();
    T &operator*();

  private:
    friend class BiList< T >;
    const typename BiList< T >::Node *curr = nullptr;
  };

  template < class T >
  class BLCIter
  {
  public:
    explicit BLCIter(const typename BiList< T >::Node *);
    bool operator==(const BLCIter< T > &) const;
    bool operator!=(const BLCIter< T > &) const;
    BLCIter< T > &operator++();
    BLCIter< T > &operator--();
    const T &operator*();

  private:
    friend class BiList< T >;
    const typename BiList< T >::Node *curr = nullptr;
  };

  template < class T >
  class BiList
  {
  public:
    struct Node
    {
      T value;
      Node *next = nullptr;
      Node *prev = nullptr;
    };
    BiList() = default;

    BiList(const BiList< T > &);
    BiList(BiList< T > &&);

    ~BiList();

    BiList< T > &operator=(const BiList< T > &);
    BiList< T > &operator=(BiList< T > &&);

    BLIter< T > begin();
    BLCIter< T > cbegin() const noexcept;

    BLIter< T > end();
    BLCIter< T > cend() const noexcept;

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    void push_front(const T &);
    void push_front(T &&);
    void push_back(const T &);
    void push_back(T &&);

    void pop_front();
    void pop_back();

    void clear() noexcept;
    bool empty() const noexcept;

  private:
    Node *head = nullptr;
    Node *tail = nullptr;
  };
}

template < class T >
alisov::BLIter< T >::BLIter(typename BiList< T >::Node *node):
  curr(node)
{}

template < class T >
bool alisov::BiList< T >::empty() const noexcept
{
  return !head;
}

template < class T >
alisov::BiList< T >::~BiList()
{
  clear();
}

template < class T >
alisov::BLCIter< T > alisov::BiList< T >::cend() const noexcept
{
  return alisov::BLCIter< T >(nullptr);
}
template < class T >
alisov::BiList< T >::BiList(BiList< T > &&other):
  head(other.head),
  tail(other.tail)
{
  other.head = nullptr;
  other.tail = nullptr;
}

template < class T >
alisov::BiList< T > &alisov::BiList< T >::operator=(BiList< T > &&other)
{
  if (this == &other) {
    return *this;
  }
  clear();
  head = other.head;
  tail = other.tail;
  other.head = nullptr;
  other.tail = nullptr;
  return *this;
}

template < class T >
bool alisov::BLIter< T >::operator!=(const BLIter< T > &other) const
{
  return !(*this == other);
}

template < class T >
bool alisov::BLCIter< T >::operator==(const BLCIter< T > &other) const
{
  return curr == other.curr;
}

template < class T >
bool alisov::BLCIter< T >::operator!=(const BLCIter< T > &other) const
{
  return !(*this == other);
}

template < class T >
void alisov::BiList< T >::clear() noexcept
{
  while (head) {
    Node *next = head->next;
    delete head;
    head = next;
  }
  head = nullptr;
  tail = nullptr;
}

template < class T >
alisov::BLIter< T > alisov::BiList< T >::begin()
{
  return alisov::BLIter< T >{head};
}

template < class T >
alisov::BLIter< T > alisov::BiList< T >::end()
{
  return alisov::BLIter< T >{nullptr};
}

template < class T >
bool alisov::BLIter< T >::operator==(const alisov::BLIter< T > &other) const
{
  return curr == other.curr;
}

template < class T >
bool alisov::BLIter< T >::operator!=(const alisov::BLIter< T > &other) const
{
  return !(*this == other);
}

template < class T >
alisov::BLIter< T > &alisov::BLIter< T >::operator++()
{
  if (!curr) {
    throw std::out_of_range("Out of range");
  }
  curr = curr->next;
  return *this;
}

template < class T >
alisov::BLIter< T > &alisov::BLIter< T >::operator--()
{
  if (!curr) {
    throw std::out_of_range("Out of range");
  }
  curr = curr->prev;
  return *this;
}

template < class T >
T const &alisov::BLCIter< T >::operator*()
{
  if (!curr) {
    throw std::logic_error("Null iterator");
  }
  return curr->value;
}

template < class T >
void alisov::BiList< T >::push_back(T &&value)
{
  if (!head) {
    head = nullptr
  }
  try {
    head = new Node{std::move(value)};
    tail = head;
  } catch (...) {
    deleat head;
    throw;
  }
  else
  {
    Node *next = nullptr;
  }
  try {
    next = new Node{std::move(value), nullptr, tail};
    tail->next = next;
    tail = next;
  } catch (...) {
    delete next;
    throw;
  }
}

template < class T >
void alisov::BiList< T >::push_back(const T &value)
{
  if (!head) {
    head = nullptr
  }
  try {
    head = new Node{value};
    tail = head;
  } catch (...) {
    deleat head;
    throw;
  }
  else
  {
    Node *next = nullptr;
  }
  try {
    next = new Node{value, nullptr, tail};
    tail->next = next;
    tail = next;
  } catch (...) {
    delete next;
    throw;
  }
}

template < class T >
void alisov::BiList< T >::push_front(T &&value)
{
  if (!head) {
    head = nullptr
  }
  try {
    head = new Node{std::move(value)};
    tail = head;
  } catch (...) {
    deleat head;
    throw;
  }
  else
  {
    Node *prev = nullptr;
  }
  try {
    prev = new Node{std::move(value), head, nullptr};
    head->prev = prev;
    head = prev;
  } catch (...) {
    delete prev;
    throw;
  }
}

template < class T >
void alisov::BiList< T >::push_front(const T &value)
{
  if (!head) {
    head = nullptr
  }
  try {
    head = new Node{value};
    tail = head;
  } catch (...) {
    deleat head;
    throw;
  }
  else
  {
    Node *prev = nullptr;
  }
  try {
    prev = new Node{value, head, nullptr};
    head->prev = prev;
    head = prev;
  } catch (...) {
    delete prev;
    throw;
  }
}

template < class T >
T &alisov::BiList< T >::front()
{
  if (!head) {
    throw std::out_of_range("Empty list");
  }
  return head->value;
}

template < class T >
const T &alisov::BiList< T >::front() const
{
  if (!head) {
    throw std::out_of_range("Empty list");
  }
  return head->value;
}

template < class T >
T &alisov::BiList< T >::back()
{
  if (!head) {
    throw std::out_of_range("Empty list");
  }
  return tail->value
}

template < class T >
const T &alisov::BiList< T >::back() const
{
  if (!head) {
    throw std::out_of_range("Empty list");
  }
  return tail->value
}

template < class T >
void alisov::BiList< T >::pop_front()
{
  if (!head) {
    throw std::out_of_range("Empty list");
  }
  Node *newHead = head->next;
  deleat head;
  head = newHead;
  if (head) {
    head->prev = nullptr;
  } else {
    tail = nullptr;
  }
}

template < class T >
void alisov::BiList< T >::pop_back()
{
  if (!tail) {
    throw std::out_of_range("Empty list");
  }
  Node *newTail = tail->prev;
  deleat tail;
  tail = newTail;
  if (tail) {
    tail->next = nullptr;
  } else {
    head = nullptr;
  }
}

template < class T >
alisov::BLCIter< T > alisov::BiList< T >::cbegin() const noexcept
{
  return alisov::BLCIter< T >(head);
}

template < class T >
T &alisov::BLIter< T >::operator*()
{
  if (!curr) {
    throw std::logic_error("Null iterator");
  }
  return curr->value;
}

template < class T >
alisov::BLCIter< T > &alisov::BLCIter< T >::operator++()
{
  if (!curr) {
    throw std::logic_error("invalid iterator");
  }
  curr = curr->next;
  return *this;
}

template < class T >
alisov::BLCIter< T > &alisov::BLCIter< T >::operator--()
{
  if (!curr) {
    throw std::logic_error("invalid iterator");
  }
  curr = curr->prev;
  return *this;
}
template < class T >
alisov::BLCIter< T >::BLCIter(const typename BiList< T >::Node *node):
  curr(node)
{}

template < class T >
alisov::BiList< T >::BiList(const BiList< T > &other)
{
  try {
    Node *curr = nullptr;
    Node *otherCurr = other.head;
    while (otherCurr) {
      if (!curr) {
        head = new Node{otherCurr->value};
        curr = head;
        otherCurr = otherCurr->next;
        continue;
      }
      Node *next = new Node{otherCurr->value, nullptr, curr};
      curr->next = next;
      curr = next;
      otherCurr = otherCurr->next;
    }
    tail = curr;
  } catch (...) {
    clear();
    throw;
  }
}

template < class T >
alisov::BiList< T > &alisov::BiList< T >::operator=(const BiList< T > &other)
{
  if (this == &other) {
    return *this;
  }
  try {
    Node *curr = nullptr;
    Node *otherCurr = other.head;
    while (otherCurr) {
      if (!curr) {
        head = new Node{otherCurr->value};
        curr = head;
        otherCurr = otherCurr->next;
        continue;
      }
      Node *next = new Node{otherCurr->value, nullptr, curr};
      curr->next = next;
      curr = next;
      otherCurr = otherCurr->next;
    }
    tail = curr;
  } catch (...) {
    clear();
    throw;
  }
  return *this;
}

#endif