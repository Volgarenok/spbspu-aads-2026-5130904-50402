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

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;

    void push_front(const T&);
    void push_back(const T&);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const;
  };
}
#endif
