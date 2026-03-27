#include <iostream>
#include "../common/list.hpp"
namespace karpenkov
{
  template < class T > class Stack
  {
  private:
    karpenkov::List< T > data;

  public:
    void push(const T &rhs);
    void pop();
    T &top();
    bool empty();
  };
}
template < class T > void karpenkov::Stack< T >::push(const T &rhs)
{
  data.push_back(rhs);
}
template < class T > void karpenkov::Stack< T >::pop()
{
  data.pop_back();
}
template < class T > T &karpenkov::Stack< T >::top()
{
  return data.back();
}
template < class T > bool karpenkov::Stack< T >::empty()
{
  return data.empty();
}
