#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include "node.hpp"
#include "iterators.hpp"
namespace karpovich
{
  template < class T > class List
  {
    Node< T > *fake_;
    size_t size_;

  public:
    List();
    List(const List< T > &other);
    List(List< T > &&other) noexcept;
    List &operator=(const List< T > &other);
    List &operator=(List< T > &&other) noexcept;
    ~List();

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;

    void push_front(const T &val);
    void push_back(const T &val);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const;
  };

  template < class T >
  List< T >::List():
    fake_(new Node< T >{T(), nullptr, nullptr}),
    size_(0)
  {
    fake_->next = fake_;
    fake_->prev = fake_;
  }

  template < class T >
  List< T >::List(const List< T > &other):
    fake_(new Node< T >{T(), nullptr, nullptr}),
    size_(0)
  {
    fake_->next = fake_;
    fake_->prev = fake_;
    Node< T > *cur = other.fake_->next;
    while (cur != other.fake_) {
      push_back(cur->val);
      cur = cur->next;
    }
  }

  template < class T >
  List< T >::List(List< T > &&other) noexcept:
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = new Node< T >{T(), nullptr, nullptr};
    other.fake_->next = other.fake_;
    other.fake_->prev = other.fake_;
    other.size_ = 0;
  }

  template < class T > List< T > &List< T >::operator=(const List< T > &other)
  {
    if (this != &other) {
      clear();
      Node< T > *cur = other.fake_->next;
      while (cur != other.fake_) {
        push_back(cur->val);
        cur = cur->next;
      }
    }
    return *this;
  }

  template < class T > List< T > &List< T >::operator=(List< T > &&other) noexcept
  {
    if (this != &other) {
      clear();
      delete fake_;
      fake_ = other.fake_;
      size_ = other.size_;
      other.fake_ = new Node< T >{T(), nullptr, nullptr};
      other.fake_->next = other.fake_;
      other.fake_->prev = other.fake_;
      other.size_ = 0;
    }
    return *this;
  }

  template < class T > List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template < class T > size_t List< T >::size() const
  {
    return size_;
  }

  template < class T > void List< T >::clear()
  {
    Node< T > *cur = fake_->next;
    while (cur != fake_) {
      Node< T > *temp = cur->next;
      delete cur;
      cur = temp;
    }
    fake_->next = fake_;
    fake_->prev = fake_;
    size_ = 0;
  }

  template < class T > void List< T >::push_back(const T &val)
  {
    Node< T > *node = new Node< T >{val, fake_, fake_->prev};
    fake_->prev->next = node;
    fake_->prev = node;
    size_++;
  }

  template < class T > void List< T >::push_front(const T &val)
  {
    Node< T > *node = new Node< T >{val, fake_->next, fake_};
    fake_->next->prev = node;
    fake_->next = node;
    size_++;
  }

  template < class T > void List< T >::pop_back()
  {
    if (size_ == 0) {
      return;
    }
    Node< T > *temp = fake_->prev;
    temp->prev->next = fake_;
    fake_->prev = temp->prev;
    delete temp;
    size_--;
  }

  template < class T > void List< T >::pop_front()
  {
    if (size_ == 0) {
      return;
    }
    Node< T > *temp = fake_->next;
    temp->next->prev = fake_;
    fake_->next = temp->next;
    delete temp;
    size_--;
  }

  template < class T > LIter< T > List< T >::begin()
  {
    return LIter< T >{fake_->next};
  }

  template < class T > LIter< T > List< T >::end()
  {
    return LIter< T >{fake_};
  }

  template < class T > LCIter< T > List< T >::begin() const
  {
    return LCIter< T >{fake_->next};
  }

  template < class T > LCIter< T > List< T >::end() const
  {
    return LCIter< T >{fake_};
  }
}
#endif
