#include "bilist.hpp"
#include <new>
#include <utility>

template < class T >
shirokov::BiList< T >::BiList(const BiList< T >& other)
{
  try
  {
    Node* curr = nullptr;
    Node* otherCurr = other.head;
    while (otherCurr)
    {
      if (!curr)
      {
        head = new Node{otherCurr->value, nullptr, nullptr};
        curr = head;
        otherCurr = otherCurr->next;
        continue;
      }
      Node* next = new Node{otherCurr->value, curr, nullptr};
      curr->next = next;
      curr = next;
      otherCurr = otherCurr->next;
    }
    tail = curr;
  }
  catch (const std::bad_alloc&)
  {
    clear();
    throw;
  }
}

template < class T >
shirokov::BiList< T >::BiList(BiList< T >&& other): head(other.head), tail(other.tail)
{
  other.head = nullptr;
  other.tail = nullptr;
}

template < class T >
shirokov::BiList< T >::~BiList() noexcept
{
  clear();
}

template < class T >
shirokov::BiList< T >& shirokov::BiList< T >::operator=(const BiList< T >& other)
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  return *this;
}

template < class T >
shirokov::BiList< T >& shirokov::BiList< T >::operator=(BiList< T >&& other)
{
  if (this == &other)
  {
    return *this;
  }
  clear();
  head = other.head;
  tail = other.tail;
  other.head = nullptr;
  other.tail = nullptr;
  return *this;
}
