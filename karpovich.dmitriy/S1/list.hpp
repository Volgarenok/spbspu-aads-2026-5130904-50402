#ifndef LIST_HPP
#define LIST_HPP

namespace karpovich
{
  template< class T > 
  class List {
    T val;
    List< T >* prev;
    List< T >* next;
  };
}
#endif
