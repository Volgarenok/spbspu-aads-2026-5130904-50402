#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iter.hpp"
namespace khalikov
{
  template< class T >
  class List
  {
  private:
    Node< T > * h;
    void clear();
  public:
    List();
    ~List();
    List< T > & operator=(const List< T > & h);
    List(const List< T > & other);
  };
}

#endif
