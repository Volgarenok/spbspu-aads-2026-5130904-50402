#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>
#include "node.hpp"
#include "iters.hpp"

namespace dirko
{
  template < class T >
  class List
  {
  public:
    List();
    List(const List< T > &);
    List(List< T > &&) noexcept;
    List &operator=(const List< T > &);
    List &operator=(List< T > &&) noexcept;
    ~List();
    Iter< T > begin();
    Iter< T > end();
    CIter< T > cbegin() const;
    CIter< T > cend() const;
    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const noexcept;
    void swap(List< T > &) noexcept;

  private:
    Node< T > *fake_;
    Node< T > *tail_;
    size_t size_;
  };

  template < class T >
  List< T >::List():
    fake_(static_cast< Node< T > * >(::operator new(sizeof(Node< T >)))),
    tail_(nullptr),
    size_(0)
  {
    fake_->next = nullptr;
    fake_->prev = nullptr;
  }
  template < class T >
  List< T >::List(List< T > &&other) noexcept:
    fake_(other.fake_),
    tail_(other.tail_),
    size_(other.size_)
  {
    other.fake_ = nullptr;
  }
  template < class T >
  List< T >::List(const List< T > &other):
    fake_(static_cast< Node< T > * >(::operator new(sizeof(Node< T >)))),
    tail_(nullptr),
    size_(0)
  {
    fake_->next = nullptr;
    fake_->prev = nullptr;
    Node< T > *head = other.fake_->next;
    while (head) {
      push_back(head->val);
      head = head->next;
    }
  }
  template < class T >
  List< T > &List< T >::operator=(List< T > &&other) noexcept
  {
    clear();
    ::operator delete(fake_);
    fake_ = other.fake_;
    size_ = other.size_;
    tail_ = other.tail_;
    other.fake_ = nullptr;
    return *this;
  }
  template < class T >
  List< T > &List< T >::operator=(const List< T > &other)
  {
    List< T > tmp = List< T >(other);
    swap(tmp);
    return *this;
  }
  template < class T >
  List< T >::~List()
  {
    clear();
    ::operator delete(fake_);
  }
  template < class T >
  void List< T >::clear()
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
  template < class T >
  void List< T >::swap(List< T > &rhs) noexcept
  {
    Node< T > *tmp = rhs.fake_;
    rhs.fake_ = fake_;
    fake_ = tmp;
    std::swap(size_, rhs.size_);
  }
  template < class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }
  template < class T >
  void List< T >::push_front(const T &val)
  {
    Node< T > *n = new Node< T >{val, fake_->next, fake_};
    if (!size_) {
      fake_->next = n;
      tail_ = n;
    } else {
      fake_->next->prev = n;
      fake_->next = n;
    }
    ++size_;
  }
  template < class T >
  void List< T >::push_back(const T &val)
  {
    Node< T > *n = new Node< T >{val, nullptr, tail_};
    if (!size_) {
      fake_->next = n;
    } else {
      tail_->next = n;
    }
    tail_ = n;
    ++size_;
  }
  template < class T >
  void List< T >::pop_front()
  {
    Node< T > *next = fake_->next->next;
    delete fake_->next;
    fake_->next = next;
    next->prev = fake_;
    --size_;
  }
  template < class T >
  void List< T >::pop_back()
  {
    tail_ = tail_->prev;
    delete tail_->next;
    tail_->next = nullptr;
    --size_;
  }
  template < class T >
  Iter< T > List< T >::begin()
  {
    return Iter< T >{fake_->next};
  }

  template < class T >
  Iter< T > List< T >::end()
  {
    return Iter< T >{tail_};
  }

  template < class T >
  CIter< T > List< T >::cbegin() const
  {
    return CIter< T >{fake_->next};
  }

  template < class T >
  CIter< T > List< T >::cend() const
  {
    return CIter< T >{tail_};
  }
}

#endif
