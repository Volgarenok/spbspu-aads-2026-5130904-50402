#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "ListImpl.hpp"

namespace vishnevskiy
{
  template <class T>
  class Queue
  {
    public:
      void push(T rhs);
      T drop();
      bool isEmpty() const;
      T seeTop();
      Queue();
      ~Queue();
    private:
      LIter<T>* fake;
  };
}

#endif
