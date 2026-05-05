#ifndef LISTIMPL_HPP
#define LISTIMPL_HPP
#include <cstddef>
#include <string>
#include <iostream>
#include "ListTools.hpp"

namespace vishnevskiy
{
  template <class T>
  List<T>::List():
    val(T()),
    next(nullptr)
  {}

  template <class T>
  List<T>::List(T vl, List<T>* nxt):
    val(vl),
    next(nxt)
  {}

  template <class T>
  NamedList<T>::NamedList():
    name(""),
    data(nullptr),
    next(nullptr)
  {}

  template <class T>
  NamedList<T>::NamedList(std::string nm, List<T>* dt, NamedList<T>* nxt):
    name(nm),
    data(dt),
    next(nxt)
  {}

  template <class T>
  LIter<T>::LIter():
    curr(nullptr)
  {}

  template <class T>
  LIter<T>::LIter(List<T>* h):
    curr(h)
  {}

  template <class T>
  T LIter<T>::value()
  {
    return (curr->val);
  }

  template <class T>
  void LIter<T>::set(List<T>* h)
  {
    curr = h;
  }

  template <class T>
  void LIter<T>::operator++()
  {
    set(curr->next);
  }

  template <class T>
  bool LIter<T>::hasNext()
  {
    if (curr)
    {
      return curr -> next;
    }
    return false;
  }

  template <class T>
  void LIter<T>::end()
  {
    while (hasNext())
    {
      ++(*this);
    }
  }

  template <class T>
  void LIter<T>::insert(T& d)
  {
    List<T>* s = new List<T>{d, nullptr};
    if (curr)
    {
      s -> next = curr -> next;
      curr -> next = s;
    }
    else
    {
      curr = s;
    }
  }

  template <class T>
  void LIter<T>::clear(LIter<T>* head)
  {
    List<T>* c = head -> curr;
    while (c)
    {
      List<T>* next = c -> next;
      delete c;
      c = next;
    }
    head -> curr = nullptr;
  }

  template <class T>
  template <class C>
  void LIter<T>::traverse(C func)
  {
    while (curr)
    {
      func(this -> value());
      ++curr;
    }
  }

  template <class T>
  NamedLIter<T>::NamedLIter():
    curr(nullptr)
  {}

  template <class T>
  NamedLIter<T>::NamedLIter(NamedList<T>* h):
    curr(h)
  {}

  template <class T>
  List<T>* NamedLIter<T>::value()
  {
    return (curr -> data);
  }

  template <class T>
  std::string NamedLIter<T>::getName()
  {
    return curr -> name;
  }

  template <class T>
  void NamedLIter<T>::setCurr(NamedList<T>* h)
  {
    curr = h;
  }

  template <class T>
  void NamedLIter<T>::operator++()
  {
    setCurr(curr->next);
  }

  template <class T>
  bool NamedLIter<T>::hasNext()
  {
    if (curr)
    {
      return curr -> next;
    }
    return false;
  }

  template <class T>
  void NamedLIter<T>::setData(List<T>* dta)
  {
    curr -> data = dta;
  }

  template <class T>
  void NamedLIter<T>::end()
  {
    while (hasNext())
    {
      ++(*this);
    }
  }

  template <class T>
  void NamedLIter<T>::insert(List<T>* d, std::string name)
  {
    NamedList<T>* s = new NamedList<T>{name, d, nullptr};
    if (curr)
    {
      s -> next = curr -> next;
      curr -> next = s;
    }
    else
    {
      curr = s;
    }
  }

  template <class T>
  void NamedLIter<T>::clear(NamedLIter<T>* head)
  {
    NamedList<T>* c = head -> curr;
    while (c)
    {
      NamedList<T>* next = c -> next;
      delete c;
      c = next;
    }
    head -> curr = nullptr;
  }

  template <class T>
  template <class C>
  void NamedLIter<T>::traverse(C func)
  {
    while (curr)
    {
      func(this -> value());
      ++(*this);
    }
  }

  template <class T>
  LCIter<T>::LCIter():
    curr(nullptr)
  {}

  template <class T>
  LCIter<T>::LCIter(const List<T>* h):
    curr(h)
  {}

  template <class T>
  void LCIter<T>::operator++()
  {
    curr = curr -> next;
  }

  template <class T>
  const T* LCIter<T>::value()
  {
    return *(curr->val);
  }

  template <class T>
  bool LCIter<T>::hasNext()
  {
    if (curr)
    {
      return curr -> next;
    }
    return false;
  }
}
#endif
