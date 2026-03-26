#ifndef POZDNYAKOV_QUEUE_HPP
#define POZDNYAKOV_QUEUE_HPP

#include "../common/list.hpp"

namespace pozdnyakov
{
  template < typename T >
  class Queue
  {
  private:
    List< T > container;
    LIter< T > tail;

  public:
    Queue():
      tail(container.end())
    {}

    Queue(const Queue &other):
      container(other.container)
    {
      if (container.empty()) {
        tail = container.end();
      } else {
        tail = container.begin();
        LIter< T > next = tail;
        ++next;
        while (next != container.end()) {
          ++tail;
          ++next;
        }
      }
    }

    Queue &operator=(const Queue &other)
    {
      if (this != &other) {
        container = other.container;
        if (container.empty()) {
          tail = container.end();
        } else {
          tail = container.begin();
          LIter< T > next = tail;
          ++next;
          while (next != container.end()) {
            ++tail;
            ++next;
          }
        }
      }
      return *this;
    }

    void push(const T &val)
    {
      if (empty()) {
        container.pushFront(val);
        tail = container.begin();
      } else {
        container.insertAfter(tail, val);
        ++tail;
      }
    }

    void pop()
    {
      container.popFront();
      if (container.empty()) {
        tail = container.end();
      }
    }

    T &front()
    {
      return container.front();
    }

    const T &front() const
    {
      return container.front();
    }

    bool empty() const noexcept
    {
      return container.empty();
    }
  };
}

#endif
