#include "../common/iters.hpp"
#include "../common/list.hpp"
#include "../common/node.hpp"

namespace strelnikov
{
  template < class T >
  class Stack
  {
  public:
    void push(const T &);
    void pop() noexcept;
    T &get() noexcept;
    bool empty() const noexcept;

  private:
    List< T > data_;
    LIter< T > head_;
  };

  template < class T >
  class Queue
  {
  public:
    void push(const T &);
    void pop() noexcept;
    T &get() noexcept;
    bool empty() const noexcept;

  private:
    List< T > data_;
  };

  template < class T >
  void Stack< T >::push(const T &val)
  {
    return;
  }

  template < class T >
  void Stack< T >::pop() noexcept
  {
    return;
  }

  template < class T >
  T &Stack< T >::get() noexcept
  {
    return T();
  }

  template < class T >
  bool Stack< T >::empty() const noexcept
  {
    return true;
  }

  template < class T >
  void Queue< T >::push(const T &val)
  {
    return;
  }

  template < class T >
  void Queue< T >::pop() noexcept
  {
    return;
  }

  template < class T >
  T &Queue< T >::get() noexcept
  {
    return T();
  }

  template < class T >
  bool Queue< T >::empty() const noexcept
  {
    return true;
  }
}
