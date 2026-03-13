#ifndef STACK_HPP
#define STACK_HPP

namespace karpovich
{
  template< typename T >
  class Queue {
  public:
    void push(T rhs);
    T drop();
  };
}

#endif
