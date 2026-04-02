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
    private:
      LIter<T>* fake;
      LIter<T>* last;
  };
}
