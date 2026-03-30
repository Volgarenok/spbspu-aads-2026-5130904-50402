#include "list.hpp"
#include <algorithm>

template class alisov::BiList< int >;
template class alisov::BLIter< int >;
template class alisov::BLCIter< int >;

template < class T > bool alisov::BiList< T >::empty() const noexcept
{
  return !head;
}

template < class T > alisov::BiList< T >::~BiList() noexcept
{
  clear();
}

template < class T > void alisov::BiList< T >::clear() noexcept
{
  while (head)
  {
    Node *next = head->next;
    delete head;
    head = next;
  }
  head = nullptr;
  tail = nullptr;
}

template < class T > alisov::BLIter< T > alisov::BiList< T >::begin()
{
  return alisov::BLIter< T >{head};
}

template < class T > alisov::BLIter< T > alisov::BiList< T >::end()
{
  return alisov::BLIter< T >{nullptr};
}

template < class T >
bool alisov::BLIter< T >::operator==(const alisov::BLIter< T > &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool alisov::BLIter< T >::operator!=(const alisov::BLIter< T > &other) const noexcept
{
  return !(*this == other);
}

template < class T > alisov::BLIter< T > alisov::BLIter< T >::operator++()
{
  if (!curr)
  {
    throw std::out_of_range("Out of range");
  }
  curr = curr->next;
  return *this;
}

template < class T > alisov::BLIter< T > alisov::BLIter< T >::operator--()
{
  if (!curr)
  {
    throw std::out_of_range("Out of range");
  }
  curr = curr->prev;
  return *this;
}

template < class T > T const &alisov::BLCIter< T >::operator*()
{
  return curr->value;
}

template < class T > void alisov::BiList< T >::push_back(T &&)
{
  if (!head)
  {
    head = new Node(std::move(value));
    tail = head;
  }
  else
  {
    tail->next = new Node(std::move(value));
    tail->next->prev = tail;
    tail = tail->next;
  }
}

template < class T > void alisov::BiList< T >::push_back(const T &value)
{
  if (!head)
  {
    head = new Node{value, nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node *next = new Node{value, nullptr, tail};
    tail->next = next;
    tail = next;
  }
}

template < class T > void alisov::BiList< T >::push_front(T &&value)
{
  if (!head)
  {
    head = new Node{value, nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node *prev = new Node{std::move(value), head, nullptr};
    head->prev = prev;
    head = prev;
  }
}

template < class T > void alisov::BiList< T >::push_front(const T &value)
{
  if (!head)
  {
    head = new Node{value, nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node *prev = new Node{std::move(value), head, nullptr};
    head->prev = prev;
    head = prev;
  }
}

template < class T > T &alisov::BiList< T >::front()
{
  return head->value;
}

template < class T > const T &alisov::BiList< T >::front() const
{
  return head->value;
}

template < class T > T &alisov::BiList< T >::back()
{
  return tail->value
}

template < class T > const T &alisov::BiList< T >::back() const
{
  return tail->value
}
