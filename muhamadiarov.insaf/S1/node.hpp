#ifndef NODE_HPP
#define NODE_HPP
namespace muhamadiarov
{
  template <class T>
  struct Node
  {
    T data_;
    Node< T >* next_;
    Node< T >* prev_;
  };
};
#endif
