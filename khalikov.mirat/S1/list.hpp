#ifndef LIST_HPP
#define LIST_HPP

namespace khalikov
{
  template< class T > class List;
  template< class T >
  class LIter
  {
    friend class List< T >;
  };
  template< class T >
  class LCIter
  {
    friend class List< T >;
  };
  template< class T >
  class List
  {
    T val;
    List< T > * next;
  };
}

#endif
