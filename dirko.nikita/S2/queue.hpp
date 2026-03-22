#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/list.hpp"

namespace dirko
{
  template < class T >
  class Queue
  {
  public:
    void push(const T &rhs);
    T &get() noexcept;
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > data_;
  };
}
#endif
