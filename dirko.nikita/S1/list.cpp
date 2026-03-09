#include "list.hpp"
#include "node.hpp"
#include <new>
namespace dirko
{
  template < class T >
  List< T >::List():
    fake_(static_cast< Node< T > * >(::operator new(sizeof(Node< T >)))),
    size_(0)
  {
    fake_->next = nullptr;
  }
  template < class T >
  List< T >::List(List< T > &&other) noexcept:
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = nullptr;
  }
  template < class T >
  List< T >::List(const List< T > &other):
    fake_(static_cast< Node< T > * >(::operator new(sizeof(Node< T >)))),
    size_(other.size_)
  {
    fake_->next = nullptr;
    Node< T > *head = other.fake_->next;
    while (head) {
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
    while (head != nullptr) {
      Node< T > *tmp = head->next;
      delete head;
      head = tmp;
    }
    fake_->next = nullptr;
    size_ = 0;
  }

}
