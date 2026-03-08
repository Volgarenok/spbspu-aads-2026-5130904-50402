#include "bilist.hpp"

template class shirokov::BiList< int >;

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
