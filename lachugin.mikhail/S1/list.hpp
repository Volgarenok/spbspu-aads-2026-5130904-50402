#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "itters.hpp"
namespace lachugin
{
  template < class T >
    class List
  {
    Node< T >* fake;
  public:
    List();
    List(const List< T >& other);
    Node< T >* addNext(const T& val, Node< T >* h);
    Node< T >* add(const T& val);

    LIter< T > begin();
    LCIter< T > begin() const;
    LIter< T > end();
    LCIter< T > end() const;

    void clear();
  };

  template < class T >
  List< T >::List()
  {
    fake = new Node< T >;
    fake->next = fake;
  }

  template < class T >
  List< T >::List(const List< T >& other): List()
  {
    Node< T >* curr = nullptr;

    for (auto it = other.begin(); it != other.end(); ++it)
    {
      if (curr == nullptr)
        curr = add(*it);
      else
        curr = addNext(*it, curr);
    }
  }

  template < class T >
  Node< T >* List< T >::add(const T& val)
  {
    Node< T >* n = new Node< T >{val, fake->next};
    fake->next = n;
    return n;
  }

  template < class T >
  Node< T >* List< T >::addNext(const T& val, Node< T >* h)
  {
    Node< T >* n = new Node< T >{val, h->next};
    h->next = n;
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
  void List< T >::clear()
  {
    auto it = begin();

    while (it != end())
    {
      auto c = it;
      ++it;
      delete c.curr;
      fake->next = it.curr;
    }
    fake->next = fake;
  }
}
#endif
