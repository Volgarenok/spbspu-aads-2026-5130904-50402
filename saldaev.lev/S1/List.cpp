#include "List.hpp"
#include <stdexcept>
#include <string>
#include <utility>

template < class T >
saldaev::List< T >::Node::Node(const T &d, Node *n, Node *p):
  data_(d),
  next_(n),
  prev_(p)
{}

template < class T >
saldaev::List< T >::List():
  head_(nullptr),
  tail_(nullptr),
  length_(0)
{
  head_ = static_cast< Node * >(::operator new(sizeof(Node)));
  try {
    tail_ = static_cast< Node * >(::operator new(sizeof(Node)));
  } catch (...) {
    ::operator delete(head_);
    throw;
  }
  head_->next_ = tail_;
  head_->prev_ = nullptr;
  tail_->prev_ = head_;
  tail_->next_ = nullptr;
}

template < class T >
saldaev::List< T >::~List()
{
  clear();
  ::operator delete(head_);
  ::operator delete(tail_);
}

template < class T >
saldaev::List< T >::List(const List &other):
  List()
{
  try {
    for (auto it = other.begin(); it != other.end(); ++it) {
      pushBack(*it);
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
    swap(tmp);
  }
  return *this;
}

template < class T >
saldaev::List< T >::List(List &&other):
  List()
{
  swap(other);
}

template < class T >
saldaev::List< T > &saldaev::List< T >::operator=(List< T > &&other)
{
  if (this != &other) {
    swap(other);
  }
  return *this;
}

template < class T >
void saldaev::List< T >::pushFront(const T &d)
{
  Node *n = new Node(d, head_->next_, head_);
  head_->next_->prev_ = n;
  head_->next_ = n;
  ++length_;
}

template < class T >
void saldaev::List< T >::pushBack(const T &d)
{
  Node *n = new Node(d, tail_, tail_->prev_);
  tail_->prev_->next_ = n;
  tail_->prev_ = n;
  ++length_;
}

template < class T >
T saldaev::List< T >::popFront()
{
  if (length_ != 0) {
    T ret = head_->next_->data_;
    Node *tmp = head_->next_->next_;
    delete head_->next_;
    head_->next_ = tmp;
    tmp->prev_ = head_;
    --length_;
    return ret;
  }
  throw std::out_of_range("pop from empty list");
}

template < class T >
T saldaev::List< T >::popBack()
{
  if (length_ != 0) {
    T ret = tail_->prev_->data_;
    Node *tmp = tail_->prev_->prev_;
    delete tail_->prev_;
    tail_->prev_ = tmp;
    tmp->next_ = tail_;
    --length_;
    return ret;
  }
  throw std::out_of_range("pop from empty list");
}

template < class T >
void saldaev::List< T >::clear() noexcept
{
  LIter it = begin();
  while (it != end()) {
    it = erase(it);
  }
}

template < class T >
void saldaev::List< T >::swap(List &other) noexcept
{
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(length_, other.length_);
}

template < class T >
void saldaev::List< T >::insertBefore(LIter it, const T &d)
{
  typename List< T >::Node *n = new typename List< T >::Node(d, it.curr_, it.curr_->prev_);
  it.curr_->prev_->next_ = n;
  it.curr_->prev_ = n;
  ++length_;
}

template < class T >
void saldaev::List< T >::insertAfter(LIter it, const T &d)
{
  if (it == end()) {
    throw std::out_of_range("cannot insert after end()");
  }
  typename List< T >::Node *n = new typename List< T >::Node(d, it.curr_->next_, it.curr_);
  it.curr_->next_->prev_ = n;
  it.curr_->next_ = n;
  ++length_;
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::erase(LIter it)
{
  if (it == end()) {
    throw std::out_of_range("cannot erase end() iterator");
  }
  typename List< T >::Node *curr_ = it.curr_;
  typename List< T >::Node *next_ = curr_->next_;
  curr_->next_->prev_ = curr_->prev_;
  curr_->prev_->next_ = curr_->next_;
  delete curr_;
  --length_;
  return LIter(next_);
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::begin()
{
  return LIter(head_->next_);
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::end()
{
  return LIter(tail_);
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::begin() const
{
  return LCIter(head_->next_);
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::end() const
{
  return LCIter(tail_);
}

template < class T >
size_t saldaev::List< T >::size() const noexcept
{
  return length_;
}

template < class T >
saldaev::List< T >::LCIter::LCIter(typename List< T >::Node *node):
  curr_(node)
{}

template < class T >
typename saldaev::List< T >::LCIter &saldaev::List< T >::LCIter::operator++() noexcept
{
  if (curr_->next_) {
    curr_ = curr_->next_;
  }
  return *this;
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::LCIter::operator++(int) noexcept
{
  LCIter ret = *this;
  if (curr_->next_) {
    curr_ = curr_->next_;
  }
  return ret;
}

template < class T >
typename saldaev::List< T >::LCIter &saldaev::List< T >::LCIter::operator--() noexcept
{
  curr_ = curr_->prev_;
  return *this;
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::LCIter::operator--(int) noexcept
{
  LCIter ret = *this;
  curr_ = curr_->prev_;
  return ret;
}

template < class T >
bool saldaev::List< T >::LCIter::operator==(const LCIter &other) const noexcept
{
  return curr_ == other.curr_;
}

template < class T >
bool saldaev::List< T >::LCIter::operator!=(const LCIter &other) const noexcept
{
  return !(*this == other);
}

template < class T >
const T &saldaev::List< T >::LCIter::operator*() const
{
  return curr_->data_;
}

template < class T >
const T *saldaev::List< T >::LCIter::operator->() const
{
  return &(curr_->data_);
}

template < class T >
saldaev::List< T >::LIter::LIter(typename List::Node *node):
  curr_(node)
{}

template < class T >
typename saldaev::List< T >::LIter &saldaev::List< T >::LIter::operator++() noexcept
{
  if (curr_->next_) {
    curr_ = curr_->next_;
  }
  return *this;
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::LIter::operator++(int) noexcept
{
  LIter ret = *this;
  if (curr_->next_) {
    curr_ = curr_->next_;
  }
  return ret;
}

template < class T >
typename saldaev::List< T >::LIter &saldaev::List< T >::LIter::operator--() noexcept
{
  curr_ = curr_->prev_;
  return *this;
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::LIter::operator--(int) noexcept
{
  LIter ret = *this;
  curr_ = curr_->prev_;
  return ret;
}

template < class T >
bool saldaev::List< T >::LIter::operator==(const LIter &other) const noexcept
{
  return curr_ == other.curr_;
}

template < class T >
bool saldaev::List< T >::LIter::operator!=(const LIter &other) const noexcept
{
  return !(*this == other);
}

template < class T >
T &saldaev::List< T >::LIter::operator*()
{
  return curr_->data_;
}

template < class T >
T *saldaev::List< T >::LIter::operator->()
{
  return &(curr_->data_);
}

template < class T >
const T &saldaev::List< T >::LIter::operator*() const
{
  return curr_->data_;
}

template < class T >
const T *saldaev::List< T >::LIter::operator->() const
{
  return &(curr_->data_);
}

template struct saldaev::List< size_t >;
template struct saldaev::List< std::pair< std::string, saldaev::List< size_t > > >;
template class saldaev::List< saldaev::List< size_t >::LIter >;
template struct saldaev::List< int >;
