#include "list.hpp"

namespace bukreev
{
  template< class T >
  List< T >::List() noexcept
  {
    m_fake.next = nullptr;
    m_tail = nullptr;
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    List< T >* cur = m_fake.next;
    while (cur)
    {
      Node< T >* n = cur->next;
      delete cur;
      cur = n;
    }
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    Node< T >* node = new Node< T >;
    node->val = value;
    node->next = nullptr;

    if (m_tail)
    {
      m_tail->next = node;
    }
    else
    {
      m_fake->next = node;
    }
    m_tail = node;
  }
}
