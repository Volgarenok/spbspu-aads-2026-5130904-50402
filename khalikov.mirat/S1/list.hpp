#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iter.hpp"

namespace khalikov
{
  template< class T >
  class LIter;

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
    LIter< T > begin();
    LCIter< T > cbegin() const;
    bool empty() const;
    void erase_after(LIter< T > pos);
    void insert_after(LIter< T > pos, const T & value);
    void merge(List< T > & other);
    void pop_front();
    void push_front(const T & value);
    void remove(const T & value);
    void sort();
    void unique();
    void reverse();
    void print(const List< T > curr);
  };
}

#endif
