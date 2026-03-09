#include "bilist.hpp"
#include <algorithm>
#include <stdexcept>

template class shirokov::BiList< int >;
template class shirokov::BLIter< int >;
template class shirokov::BLCIter< int >;

template < class T >
bool shirokov::BiList< T >::empty() const noexcept
{
  return !head;
}

template < class T >
shirokov::BiList< T >::~BiList() noexcept
{
  clear();
}

template < class T >
void shirokov::BiList< T >::clear() noexcept
{
  while (head)
  {
    Node* next = head->next;
    delete head;
    head = next;
  }
  head = nullptr;
  tail = nullptr;
}

template < class T >
shirokov::BLIter< T > shirokov::BiList< T >::begin()
{
  return shirokov::BLIter< T >(head);
}

template < class T >
shirokov::BLIter< T > shirokov::BiList< T >::end()
{
  return shirokov::BLIter< T >(nullptr);
}

template < class T >
bool shirokov::BLIter< T >::operator==(const shirokov::BLIter< T >& other) const noexcept
{
  return curr == other.curr;
}

template < class T >
shirokov::BLIter< T >::BLIter(typename BiList< T >::Node* node): curr(node)
{
}

template < class T >
void shirokov::BiList< T >::push_back(T&& value)
{
  if (!head)
  {
    head = new Node{std::move(value), nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node* next = new Node{std::move(value), nullptr, tail};
    tail->next = next;
    tail = next;
  }
}

template < class T >
void shirokov::BiList< T >::push_back(const T& value)
{
  if (!head)
  {
    head = new Node{value, nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node* next = new Node{value, nullptr, tail};
    tail->next = next;
    tail = next;
  }
}

template < class T >
T& shirokov::BiList< T >::front()
{
  return head->value;
}

template < class T >
const T& shirokov::BiList< T >::front() const
{
  return head->value;
}

template < class T >
T& shirokov::BiList< T >::back()
{
  return tail->value;
}

template < class T >
const T& shirokov::BiList< T >::back() const
{
  return tail->value;
}
