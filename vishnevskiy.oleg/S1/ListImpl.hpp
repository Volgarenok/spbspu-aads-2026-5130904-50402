#ifndef LISTIMPL_HPP
#define LISTIMPL_HPP
#include <cstddef>
#include <string>
#include "ListTools.hpp"

namespace vishnevskiy
{
  template < class T >
  List< T >::List():
    val_(T()),
    next_(nullptr)
  {}

  template < class T >
  List< T >::List(T val, List< T >* next):
    val_(val),
    next_(next)
  {}

  template < class T >
  List< T >::List(const List< T >& other):
    val_(other.val_),
    next_(nullptr)
  {
    if (other.next_)
    {
      next_ = new List< T >(*other.next_);
    }
  }

  template < class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != &other)
    {
      val_ = other.val_;
      delete next_;
      if (other.next_)
      {
        next_ = new List< T >(*other.next_);
      }
      else
      {
        next_ = nullptr;
      }
    }
    return *this;
  }

  template < class T >
  NamedList< T >::NamedList():
    name_(""),
    data_(nullptr),
    next_(nullptr)
  {}

  template < class T >
  NamedList< T >::NamedList(std::string name, List< T >* data, NamedList< T >* next):
    name_(name),
    data_(data),
    next_(next)
  {}

  template < class T >
  LIter< T >::LIter():
    curr_(nullptr)
  {}

  template < class T >
  LIter< T >::LIter(List< T >* h):
    curr_(h)
  {}

  template < class T >
  T LIter< T >::value()
  {
    return (curr_->val_);
  }

  template < class T >
  void LIter< T >::set(List< T >* h)
  {
    curr_ = h;
  }

  template < class T >
  void LIter< T >::operator++()
  {
    set(curr_->next_);
  }

  template < class T >
  bool LIter< T >::hasNext()
  {
    if (curr_)
    {
      return curr_->next_;
    }
    return false;
  }

  template < class T >
  void LIter< T >::end()
  {
    while (hasNext())
    {
      ++(*this);
    }
  }

  template < class T >
  void LIter< T >::insert(T& d)
  {
    List< T >* s = new List< T >{d, nullptr};
    if (curr_)
    {
      s->next_ = curr_->next_;
      curr_->next_ = s;
    }
    else
    {
      curr_ = s;
    }
  }

  template < class T >
  void LIter< T >::clear(LIter< T >* head)
  {
    List< T >* c = head->curr_;
    while (c)
    {
      List< T >* next = c->next_;
      delete c;
      c = next;
    }
    head->curr_ = nullptr;
  }

  template < class T >
  template < class C >
  void LIter< T >::traverse(C func)
  {
    while (curr_)
    {
      func(this->value());
      ++curr_;
    }
  }

  template < class T >
  NamedLIter< T >::NamedLIter():
    curr_(nullptr)
  {}

  template < class T >
  NamedLIter< T >::NamedLIter(NamedList< T >* h):
    curr_(h)
  {}

  template < class T >
  List< T >* NamedLIter< T >::value()
  {
    return (curr_->data_);
  }

  template < class T >
  std::string NamedLIter< T >::getName()
  {
    return curr_->name_;
  }

  template < class T >
  void NamedLIter< T >::setCurr(NamedList< T >* h)
  {
    curr_ = h;
  }

  template < class T >
  void NamedLIter< T >::operator++()
  {
    setCurr(curr_->next_);
  }

  template < class T >
  bool NamedLIter< T >::hasNext()
  {
    if (curr_)
    {
      return curr_->next_;
    }
    return false;
  }

  template < class T >
  void NamedLIter< T >::setData(List< T >* data)
  {
    curr_->data_ = data;
  }

  template < class T >
  void NamedLIter< T >::end()
  {
    while (hasNext())
    {
      ++(*this);
    }
  }

  template < class T >
  void NamedLIter< T >::insert(List< T >* d, std::string name)
  {
    NamedList< T >* s = new NamedList< T >{name, d, nullptr};
    if (curr_)
    {
      s->next_ = curr_->next_;
      curr_->next_ = s;
    }
    else
    {
      curr_ = s;
    }
  }

  template < class T >
  void NamedLIter< T >::clear(NamedLIter< T >* head)
  {
    NamedList< T >* c = head->curr_;
    while (c)
    {
      NamedList< T >* next = c->next_;
      delete c;
      c = next;
    }
    head->curr_ = nullptr;
  }

  template < class T >
  template < class C >
  void NamedLIter< T >::traverse(C func)
  {
    while (curr_)
    {
      func(this->value());
      ++(*this);
    }
  }

  template < class T >
  LCIter< T >::LCIter():
    curr_(nullptr)
  {}

  template < class T >
  LCIter< T >::LCIter(const List< T >* h):
    curr_(h)
  {}

  template < class T >
  void LCIter< T >::operator++()
  {
    curr_ = curr_->next_;
  }

  template < class T >
  const T* LCIter< T >::value()
  {
    return (curr_->val_);
  }

  template < class T >
  bool LCIter< T >::hasNext()
  {
    if (curr_)
    {
      return curr_->next_;
    }
    return false;
  }
}
#endif
