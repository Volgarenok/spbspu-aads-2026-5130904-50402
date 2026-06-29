#ifndef NOVIKOV_QUEUE_HPP
#define NOVIKOV_QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>
#include <utility>

namespace novikov
{
  template < class T >
  class Queue
  {
    List< T > data;

  public:
    bool empty() const
    {
      return data.empty();
    }

    void push(const T &val)
    {
      data.push_back(val);
    }

    void push(T &&val)
    {
      data.push_back(std::move(val));
    }

    T drop()
    {
      if (empty()) {
        throw std::runtime_error("Queue is empty");
      }
      T val = std::move(*data.begin());
      data.pop_front();
      return val;
    }

    const T &front() const
    {
      if (empty()) {
        throw std::runtime_error("Queue is empty");
      }
      return *data.begin();
    }
  };
}
#endif
