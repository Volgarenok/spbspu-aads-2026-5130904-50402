#ifndef STACK_HPP
#define STACK_HPP
#include "../common/List.hpp"

namespace saldaev
{
  template < class T >
  struct Stack
  {
    Stack();
    Stack(const Stack &other) = default;
    Stack(Stack &&other) = default;
    ~Stack() = default;
    Stack &operator=(const Stack &other) = default;
    Stack &operator=(Stack &&other);

    void push(const T &value);
    T &top();
    const T &top() const;
    void pop() noexcept;
    void clear() noexcept;
    bool empty() const noexcept;

  private:
    List< T > data_;
  };

}

#endif
