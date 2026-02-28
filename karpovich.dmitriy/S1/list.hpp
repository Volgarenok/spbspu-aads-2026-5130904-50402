#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
namespace karpovich
{
  template< class T > 
  class List {
    Node* head;
    Node* tail;
  public:
    List< T >();
    List< const T& >;
    List< T >(List&&)
    List& operator=(const List&);
    List& operator=(List&&) noexcept;
    ~List();
  };
}
#endif
