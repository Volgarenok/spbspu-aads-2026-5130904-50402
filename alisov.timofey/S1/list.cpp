#include "list.hpp"
#include "node.hpp"
#include <new>

namespace alisov
{
  template < class T >
  List< T >::List()
      : fake_(static_cast< alisov::Node< T > * >(::operator new(sizeof(alisov::Node< T >)))),
        size_(0)
  {
    fake_->next = nullptr;
    fake_->prev = nullptr;
  }
  template < class T >
  List< T >::List(List< T > &&other) noexcept
      : fake_(other.fake_), tail_(other.tail_), size_(other.size_)
  {
    other.fake_ = nullptr;
    other.size_ = 0;
  }
  template < class T >
  List< T >::List(const List< T > &other)
      : fake_(static_cast< Node< T > * >(::operator new(sizeof(Node< T >)))), size_(other.size_),
        tail_(fake_), size_(0)
  {
    fake_->next = nullptr;
    fake_->prev = nullptr;
    Node< T > *head = other.fake_->next;
    while (head)
    {
      push_back(head);
      head = head->next;
    }
  }
  template < class T > List< T > &List< T >::operator=(List< T > &&other) noexcept
  {
    clear();
    ::operator delete(fake_);
    fake_ = other.fake_;
    size_ = other.size_;
    other.fake_ = nullptr;
    return *this;
  }
  template < class T > List< T > &List< T >::operator=(const List< T > &other)
  {
    List< T > tmp = List< T >(other);
    swap(tmp);
    return *this;
  }
  template < class T > List< T >::~List()
  {
    clear();
    ::operator delete(fake_);
  }
  template < class T > void List< T >::clear()
  {
    Node< T > *head = fake_->next;
    while (head != nullptr)
    {
      Node< T > *tmp = head->next;
      delete head;
      head = tmp;
    }
    fake_->next = nullptr;
    size_ = 0;
  }
}
