#ifndef MATVEEV_STACK_HPP
#define MATVEEV_STACK_HPP

#include "../common/list.hpp"

namespace matveev
{

template<class T>
class Stack
{
private:
  List<T> data_;
};

}

#endif
