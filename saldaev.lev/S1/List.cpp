#include "List.hpp"
#include <string>
#include <utility>

template < class T >
saldaev::List< T >::Node::Node(const T &d, Node *n, Node *p):
  data(d),
  next(n),
  prev(p)
{}

template < class T >
saldaev::List< T >::List():
  head(nullptr),
  tail(nullptr),
  length(0)
{
  head = static_cast< Node * >(::operator new(sizeof(Node)));
  try {
    tail = static_cast< Node * >(::operator new(sizeof(Node)));
  } catch (...) {
    ::operator delete(head);
    throw;
  }
  head->next = tail;
  head->prev = nullptr;
  tail->prev = head;
  tail->next = nullptr;
}

template < class T >
saldaev::List< T >::~List()
{
  clear();
  ::operator delete(head);
  ::operator delete(tail);
}

template < class T >
saldaev::List< T >::List(const List &other):
  List()
{
  try {
    for (auto it = other.begin(); it != other.end(); ++it) {
      newTail(it.getData());
    }
  } catch (...) {
    clear();
    throw;
  }
}

template < class T >
saldaev::List< T > &saldaev::List< T >::operator=(const List< T > &other)
{
  if (this != &other) {
    List< T > tmp(other);
    std::swap(head, tmp.head);
    std::swap(tail, tmp.tail);
    std::swap(length, tmp.length);
  }
  return *this;
}

template < class T >
saldaev::List< T >::List(List &&other):
  List()
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(length, other.length);
}

template < class T >
saldaev::List< T > &saldaev::List< T >::operator=(List< T > &&other)
{
  if (this != &other) {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
  }
  return *this;
}

template < class T >
void saldaev::List< T >::newHead(const T &d)
{
  Node *n = new Node(d, head->next, head);
  head->next->prev = n;
  head->next = n;
  ++length;
}

template < class T >
void saldaev::List< T >::newTail(const T &d)
{
  Node *n = new Node(d, tail, tail->prev);
  tail->prev->next = n;
  tail->prev = n;
  ++length;
}

template < class T >
void saldaev::List< T >::cutHead() noexcept
{
  if (length != 0) {
    Node *tmp = head->next->next;
    delete head->next;
    head->next = tmp;
    tmp->prev = head;
    --length;
  }
}

template < class T >
void saldaev::List< T >::cutTail() noexcept
{
  if (length != 0) {
    Node *tmp = tail->prev->prev;
    delete tail->prev;
    tail->prev = tmp;
    tmp->next = tail;
    --length;
  }
}

template < class T >
void saldaev::List< T >::clear() noexcept
{
  while (length > 0) {
    cutHead();
  }
}

template < class T >
void saldaev::List< T >::setData(LIter< T > it, const T &d)
{
  if (it.isValid()) {
    it.curr->data = d;
  }
}

template < class T >
void saldaev::List< T >::addBefore(LIter< T > it, const T &d)
{
  if (it.isValid()) {
    typename List< T >::Node *n = new typename List< T >::Node(d, it.curr, it.curr->prev);
    it.curr->prev->next = n;
    it.curr->prev = n;
    ++length;
  }
}

template < class T >
void saldaev::List< T >::addAfter(LIter< T > it, const T &d)
{
  if (it.isValid()) {
    typename List< T >::Node *n = new typename List< T >::Node(d, it.curr->next, it.curr);
    it.curr->next->prev = n;
    it.curr->next = n;
    ++length;
  }
}

template < class T >
saldaev::LIter< T > saldaev::List< T >::cutCurrent(LIter< T > it) noexcept
{
  if (it.isValid()) {
    typename List< T >::Node *curr = it.curr;
    typename List< T >::Node *next = curr->next;
    curr->next->prev = curr->prev;
    curr->prev->next = curr->next;
    delete curr;
    --length;
    return LIter< T >(next);
  }
  return end();
}

template < class T >
saldaev::LIter< T > saldaev::List< T >::begin()
{
  return LIter< T >(head->next);
}

template < class T >
saldaev::LIter< T > saldaev::List< T >::end()
{
  return LIter< T >(tail);
}

template < class T >
saldaev::LCIter< T > saldaev::List< T >::begin() const
{
  return LCIter< T >(head->next);
}

template < class T >
saldaev::LCIter< T > saldaev::List< T >::end() const
{
  return LCIter< T >(tail);
}

template < class T >
size_t saldaev::List< T >::getLength() const noexcept
{
  return length;
}

template < class T >
saldaev::LCIter< T >::LCIter(typename List< T >::Node *node):
  curr(node),
  valid(true)
{
  if (!node) {
    valid = false;
  } else if (!(node->next) || !(node->prev)) {
    valid = false;
  }
}

template < class T >
bool saldaev::LCIter< T >::isValid() const noexcept
{
  return valid;
}

template < class T >
bool saldaev::LCIter< T >::hasNext() const noexcept
{
  return isValid() && (curr->next->next != nullptr);
}

template < class T >
bool saldaev::LCIter< T >::hasPrev() const noexcept
{
  return isValid() && (curr->prev->prev != nullptr);
}

template < class T >
saldaev::LCIter< T > &saldaev::LCIter< T >::operator++() noexcept
{
  if (isValid()) {
    curr = curr->next;
    valid = curr->next;
  }
  return *this;
}

template < class T >
saldaev::LCIter< T > saldaev::LCIter< T >::operator++(int) noexcept
{
  LCIter< T > ret = *this;
  if (isValid()) {
    curr = curr->next;
    valid = curr->next;
  }
  return ret;
}

template < class T >
saldaev::LCIter< T > &saldaev::LCIter< T >::operator--() noexcept
{
  if (isValid()) {
    curr = curr->prev;
    valid = curr->prev;
  }
  return *this;
}

template < class T >
saldaev::LCIter< T > saldaev::LCIter< T >::operator--(int) noexcept
{
  LCIter< T > ret = *this;
  if (isValid()) {
    curr = curr->prev;
    valid = curr->prev;
  }
  return ret;
}

template < class T >
bool saldaev::LCIter< T >::operator==(const LCIter &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool saldaev::LCIter< T >::operator==(const LIter< T > &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool saldaev::LCIter< T >::operator!=(const LCIter &other) const noexcept
{
  return !(*this == other);
}

template < class T >
bool saldaev::LCIter< T >::operator!=(const LIter< T > &other) const noexcept
{
  return !(*this == other);
}

template < class T >
const T &saldaev::LCIter< T >::getData() const noexcept
{
  return curr->data;
}

template < class T >
saldaev::LIter< T >::LIter(typename List< T >::Node *node):
  curr(node),
  valid(true)
{
  if (!node) {
    valid = false;
  } else if (!(node->next) || !(node->prev)) {
    valid = false;
  }
}

template < class T >
bool saldaev::LIter< T >::isValid() const noexcept
{
  return valid;
}

template < class T >
bool saldaev::LIter< T >::hasNext() const noexcept
{
  return isValid() && (curr->next->next != nullptr);
}

template < class T >
bool saldaev::LIter< T >::hasPrev() const noexcept
{
  return isValid() && (curr->prev->prev != nullptr);
}

template < class T >
saldaev::LIter< T > &saldaev::LIter< T >::operator++() noexcept
{
  if (isValid()) {
    curr = curr->next;
    valid = curr->next;
  }
  return *this;
}

template < class T >
saldaev::LIter< T > saldaev::LIter< T >::operator++(int) noexcept
{
  LIter< T > ret = *this;
  if (isValid()) {
    curr = curr->next;
    valid = curr->next;
  }
  return ret;
}

template < class T >
saldaev::LIter< T > &saldaev::LIter< T >::operator--() noexcept
{
  if (isValid()) {
    curr = curr->prev;
    valid = curr->prev;
  }
  return *this;
}

template < class T >
saldaev::LIter< T > saldaev::LIter< T >::operator--(int) noexcept
{
  LIter< T > ret = *this;
  if (isValid()) {
    curr = curr->prev;
    valid = curr->prev;
  }
  return ret;
}

template < class T >
bool saldaev::LIter< T >::operator==(const LIter &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool saldaev::LIter< T >::operator==(const LCIter< T > &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool saldaev::LIter< T >::operator!=(const LIter &other) const noexcept
{
  return !(*this == other);
}

template < class T >
bool saldaev::LIter< T >::operator!=(const LCIter< T > &other) const noexcept
{
  return !(*this == other);
}

template < class T >
const T &saldaev::LIter< T >::getData() const noexcept
{
  return curr->data;
}

template < class T >
T &saldaev::LIter< T >::getData() noexcept
{
  return curr->data;
}

template struct saldaev::List< size_t >;
template struct saldaev::LIter< size_t >;
template struct saldaev::LCIter< size_t >;

template struct saldaev::List< std::pair< std::string, saldaev::List< size_t > > >;
template struct saldaev::LIter< std::pair< std::string, saldaev::List< size_t > > >;
template struct saldaev::LCIter< std::pair< std::string, saldaev::List< size_t > > >;

template class saldaev::List< saldaev::LIter< size_t > >;
template struct saldaev::LIter< saldaev::LIter< size_t > >;
template struct saldaev::LCIter< saldaev::LIter< size_t > >;

template struct saldaev::List< int >;
template struct saldaev::LIter< int >;
template struct saldaev::LCIter< int >;
