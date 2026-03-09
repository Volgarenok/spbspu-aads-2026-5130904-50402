#ifndef LIST_HPP
#define LIST_HPP

namespace khalikov
{
  template< class T >
  class List
  {
    T val;
    List< T > * next;
  };
}

#endif
