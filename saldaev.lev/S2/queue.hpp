#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../common/List.hpp"

namespace saldaev
{
  template < class T >
  struct Queue
  {
    Queue();
    Queue(const Queue &other);
    Queue(Queue &&other);
    ~Queue();
    Queue &operator=(const Queue &other);
    Queue &operator=(Queue &&other);

    void push(const T &value);
    T &front();
    const T &front() const;
    void pop() noexcept;
    void clear() noexcept;
    bool empty() const noexcept;

  private:
    List< T > data_;
  };

}

#endif
