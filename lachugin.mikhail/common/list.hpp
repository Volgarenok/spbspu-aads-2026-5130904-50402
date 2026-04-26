#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include "node.hpp"
#include "itters.hpp"
namespace lachugin
{
  template < class T >
    class List
  {
    Node< T >* fake;
    size_t size_;
  public:
    List();
    ~List();
    List(const List< T >& other);
    List &operator=(const List< T > &other);

    Node< T >* addNext(const T& val, Node< T >* h);
    Node< T >* add(const T& val);
    void pushBack(const T& val);

    LIter< T > begin();
    LCIter< T > begin() const;
    T& front();
    const T& front() const;
    T &back();
    const T &back() const;
    LIter< T > end();
    LCIter< T > end() const;

    size_t size() const;
    bool empty() const;
    void clear();
    void popFront();
    void popEnd();
    Node<T>* getTail();

  };

  template < class T >
  List< T >::List()
  {
    fake = new Node< T >;
    fake->next = fake;
    size_ = 0;
  }

  template < class T >
  List< T >::List(const List< T >& other): List()
  {
    Node< T >* curr = nullptr;

    for (auto it = other.begin(); it != other.end(); ++it)
    {
      if (curr == nullptr)
      {
        curr = add(*it);
      }
      else
      {
        curr = addNext(*it, curr);
      }
    }
  }

  template < class T >
  List<T>::~List()
  {
    clear();
    delete fake;
  }

  template < class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != other)
    {
      clear();
      auto itO = other.begin();
      auto itT = this->begin();

      while (itO != other.end())
      {
        Node< T >* n = nullptr;
        if (itT == fake)
        {
          n = this->add(*itO);
        }
        else
        {
          n = this->addNext(*itO, n);
        }
        ++itO;
      }
    }
    return* this;
  }

  template < class T >
  Node< T >* List< T >::add(const T& val)
  {
    Node< T >* n = new Node< T >{val, fake->next};
    fake->next = n;
    size_++;
    return n;
  }

  template < class T >
  Node< T >* List< T >::addNext(const T& val, Node< T >* h)
  {
    Node< T >* n = new Node< T >{val, h->next};
    h->next = n;
    size_++;
    return n;
  }

  template < class T >
  LIter< T > List< T >::begin()
  {
    return LIter< T > {fake->next};
  }

  template < class T >
  LCIter< T > List< T >::begin() const
  {
    return LCIter< T > {fake->next};
  }

  template < class T >
  T& List< T >::front()
  {
    return fake->next->value;
  }

  template < class T >
  const T& List< T >::front() const
  {
    return fake->next->value;
  }

  template< class T >
  T& List< T >::back()
  {
    auto it = this->begin();
    while (it.curr->next != fake)
    {
      ++it;
    }
    return it.curr->value;
  }

  template< class T >
  const T& List< T >::back() const
  {
    auto it = this->begin();
    while (it.curr->next != fake)
    {
      ++it;
    }
    return it.curr->value;
  }

  template < class T >
  LIter< T > List< T >::end()
  {
    return LIter< T > {fake};
  }

  template < class T >
  LCIter< T > List< T >::end() const
  {
    return LCIter< T > {fake};
  }

  template < class T >
  size_t List< T >::size() const
  {
    return size_;
  }

  template < class T >
  bool List< T >::empty() const
  {
    return size_ == 0;
  }

  template < class T >
  void List< T >::clear()
  {
    Node< T >* curr = fake->next;

    while (curr != fake)
    {
      Node< T >* next = curr->next;
      delete curr;
      curr = next;
    }
    fake->next = fake;
    size_ = 0;
  }

  template < class T >
  void List< T >::popFront()
  {
    if (empty())
    {
      throw std::out_of_range("Queue is empty");
    }

    Node< T >* curr = fake->next;

    if (curr->next == fake)
    {
      delete curr;
      fake->next = fake;
    }
    else
    {
      fake->next = curr->next;
      delete curr;
    }

    size_--;
  }

  template< class T >
  void List< T >::popEnd()
  {
    if (empty())
    {
      throw std::out_of_range("Queue is empty");
    }
    Node< T >* curr = fake->next;
    while (curr->next->next != fake)
    {
      curr = curr->next;
    }
    Node< T >* n = curr;
    curr = curr->next;
    delete curr;
    size_--;
    n->next = fake;
  }

  template < class T >
  Node< T >* List<T>::getTail()
  {
    if (empty())
    {
      return fake;
    }

    Node< T >* it = fake->next;
    while (it->next != fake)
    {
      it = it->next;
    }
    return it;
  }

  template < class T >
  void List< T >::pushBack(const T& val)
  {
    Node< T >* it = fake;

    while (it->next != fake)
    {
      it = it->next;
    }

    Node< T >* n = new Node< T >{val, fake};
    it->next = n;
    size_++;
  }
}
#endif
