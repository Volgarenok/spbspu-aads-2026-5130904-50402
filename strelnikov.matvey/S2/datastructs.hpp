#include "../common/iters.hpp"
#include "../common/list.hpp"
#include "../common/node.hpp"

namespace strelnikov
{
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
  class Stack
  {
  public:
    void push(const T &);
    void pop() noexcept;
    T &get() noexcept;
    bool empty() const noexcept;

  private:
    List< T > data_;
  };
}
