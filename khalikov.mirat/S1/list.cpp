#ifndef LIST_CPP
#define LIST_CPP
#include "node.hpp"

namespace khalikov
{
  template< class T >
  class List
  {
  private:
    Node< T > * h;
  public:
    List();
    ~List()
    {
      clear(h);
    }
    List operator=(const List< T > & other);
  };
}

#endif
