#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iter.hpp"
#include "citer.hpp"

namespace khalikov
{
  template< class T >
  class LCIter;

  template< class T >
  class LIter;

  template< class T >
  class List
  {
  private:
    Node< T > * h;
  public:
    List();
    ~List();
    List< T > & operator=(const List< T > & h);
    List(const List< T > & other);
    LIter< T > begin();
    LCIter< T > cbegin() const;
    void swap(List< T > & other) noexcept;
		void clear();
    bool isEmpty() const noexcept;
    void eraseAfter(LIter< T > pos);
    void insertAfter(LIter< T > pos, const T & value);
    void merge(List< T > & other);
    void popFront();
    void pushFront(const T & value);
    void erase(LIter< T > pos);
    void remove(const T & value);
    void sort();
    void unique();
    void reverse();
    void print(const List< T > curr);
  };
}

template< class T >
khalikov::List< T >::List():
  h(nullptr)
{}

template< class T >
khalikov::List< T >::~List()
{
  clear();
}

template< class T >
List< T >::List(const List< T > & other)
{
  if(!other.h)
  {
    h = nullptr;
    return;
  }

	try
	{
	  h = new Node< T > {other.h->val, nullptr};
	  Node< T > * curr = h;
	  Node< T > * curr_other = other.h->next;

	  while(curr_other)
	  {
	    curr->next = new Node< T > {curr_other->val, nullptr};
	    curr = curr->next;
	    curr_other = curr_other->next;
	  }
	}
	catch (...)
	{
		clear();
		throw;
	}
}


template< class T >
khalikov::List< T > & khalikov::List< T >::operator=(const List< T > & other)
{
  if (this != addressof(other))
  {
    List< T > cpy(other);
    swap(cpy);
  }
  return *this;
}

template< class T >
void khalikov::List< T >::swap(List< T > & other) noexcept
{
	std::swap(h, other.h);
}

template< class T >
void khalikov::List< T >::clear()
{
  while(h)
  {
    Node< T > * temp = h->next;
    delete h;
    h = temp;
  }
}

template< class T >
khalikov::LIter< T > khalikov::List< T >::begin()
{
  return LIter< T >(h);
}

template< class T >
khalikov::LCIter< T > khalikov::List< T >::cbegin() const
{
  return LCIter< T >(h);
}

template< class T >
bool khalikov::List< T >::isEmpty() const noexcept
{
  return h == nullptr;
}

template< class T >
void khalikov::List< T >::eraseAfter(LIter< T > pos)
{
  Node< T > * erase = pos.curr->next;
  pos.curr->next = erase->next;
  delete erase;
}

template< class T >
void khalikov::List< T >::insertAfter(LIter< T > pos, const T & value)
{
  Node< T > * new_element = new Node< T > {value, pos.curr->next};
  pos.curr->next = new_element;
}

template< class T >
void khalikov::List< T >::merge(List< T > & other)
{
  Node< T > * res_h = nullptr;
  if (h->val > other.h->val)
  {
    res_h = other.h;
    other.h = other.h->next;
  }
  else
  {
    res_h = h;
    h = h->next;
  }
  Node< T > * res_tail = res_h;
  res_tail->next = nullptr;
  while (h && other.h)
  {
    if (h->val > other.h->val)
    {
      res_tail->next = other.h;
      other.h = other.h->next;
    }
    else
    {
      res_tail->next = h;
      h = h->next;
    }
    res_tail = res_tail->next;
    res_tail->next = nullptr;
  }
  if (h)
  {
    res_tail->next = h;
  }
  if (other.h)
  {
    res_tail->next = other.h;
  }
  other.h = nullptr;
  h = res_h;
}

template< class T >
void khalikov::List< T >::popFront()
{
  Node< T > * temp = h;
  h = h->next;
  delete temp;
}

template< class T >
void khalikov::List< T >::pushFront(const T & value)
{
  Node< T > * temp = new Node< T > {value, h};
  h = temp;
}

template< class T >
void khalikov::List< T >::erase(LIter< T > pos)
{
  Node< T > * prev = h;
  while (prev->next != pos.curr)
  {
    prev = prev->next;
  }
  prev->next = pos.curr->next;
  delete pos.curr;
}

template< class T >
void khalikov::List< T >::remove(const T & value)
{
  auto it = begin();
  while (it.curr != nullptr)
  {
    auto next = it;
    next++;
    if (it.curr->val == value)
    {
      erase(it);
    }
    it = next;
  }
}

template< class T >
void List< T >::sort()
{
  for (auto i = begin(); i.curr != nullptr; ++i)
  {
    for (auto j = i; j.curr != nullptr; ++j)
    {
      if (i.curr->val > j.curr->val)
      {
      	std::swap(i.curr->val, j.curr->val);
      }
    }
  }
}

#endif
