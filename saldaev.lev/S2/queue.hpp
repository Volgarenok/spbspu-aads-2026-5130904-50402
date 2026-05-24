#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/List.hpp"

namespace saldaev
{
  template< class T >
  struct Queue
  {
    void push(const T &value);
    void push(T &&value);
    T &front();
    const T &front() const;
    void pop();
    void clear() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    List< T > data_;
  };

}

#endif
