#ifndef NODE_HPP
#define NODE_HPP
namespace muhamadiarov
{
  template <class T>
  struct Node
  {
    T data;
    Node< T >* next;
    Node< T >* prev;
  };
};
#endif
