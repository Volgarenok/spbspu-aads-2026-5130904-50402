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
    List();
    List(const List< T >&);
    List(List< T >&&)
    List& operator=(const List< T >&);
    List& operator=(List< T >&&);
    ~List();
  };
}
#endif
