#ifndef MATVEEV_QUEUE_HPP
#define MATVEEV_QUEUE_HPP

#include "../common/list.hpp"

namespace matveev
{

template<class T>
class Queue
{
private:
  List<T> data_;
  LIter<T> tail_;
};

}

#endif
