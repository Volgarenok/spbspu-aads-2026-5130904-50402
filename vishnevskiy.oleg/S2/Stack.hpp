#ifndef STACK_HPP
#define STACK_HPP
#include "ListImpl.hpp"

namespace vishnevskiy
{
  template <class T>
  class Stack
  {
    public:
      void push(T rhs);
      T drop();
      bool isEmpty() const;
      T seeTop();
      Stack();
      ~Stack();
    private:
      LIter<T>* fake;
  };
}

#endif
