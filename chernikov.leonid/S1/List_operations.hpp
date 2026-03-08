#ifndef LIST_OPERATIONS_HPP
#define LIST_OPERATIONS_HPP

#include "List.hpp"
namespace chernikov {
  template < typename T > void List< T >::copy_from(const List &other)
  {
    if (!other.head)
    {
      return;
    }
    head = new Node< T >{other.head->data, nullptr};
    count = 1;
    Node< T > *current = head;
    Node< T > *other_current = other.head->next;
    while (other_current)
    {
      current->next = new Node< T >{other_current->data, nullptr};
      current = current->next;
      other_current = other_current->next;
      ++count;
    }
  }
  template < typename T > void List< T >::add(const T &val)
  {
    head = new Node< T >(val, head);
    ++count;
  }
  template < typename T > void List< T >::first_delete()
  {
    if (empty())
    {
      throw std::logic_error("List is empty");
    }
    Node< T > *old = head;
    head = head->next;
    delete old;
    --count;
  }
  template < typename T > void List< T >::clear()
  {
    while (!empty())
    {
      first_delete();
    }
  }
  template < typename T > List< T > &List< T >::operator=(const List &other)
  {
    if (this != &other)
    {
      List tmp(other);
      swap(tmp);
    }
  }
  template < typename T > List< T > &List< T >::operator=(List &&other) noexcept
  {
    if (this != &other)
    {
      clear();
      head = other.head;
      count = other.count;
      other.head = nullptr;
      other.count = 0;
    }
    return *this;
  }
}
#endif
