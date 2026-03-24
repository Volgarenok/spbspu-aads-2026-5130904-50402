#ifndef QUEUE_HPP
#define QUEUE_HPP

namespace afanasev
{
  template< typename T >
  class Queue
  {
   public:
    void push(T rhs);
    T drop();
  };
}

#endif
