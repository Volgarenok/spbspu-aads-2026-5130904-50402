#include "stack.hpp"
#include <stdexcept>

namespace saldaev
{
  template < class T >
  Stack< T >::Stack():
    data_()
  {}

  template < class T >
  bool Stack< T >::empty() const noexcept
  {
    return false;
  }

  template < class T >
  void Stack< T >::push(const T &value)
  {}

  template < class T >
  T &Stack< T >::top()
  {
    auto dummy = T();
    return dummy;
  }

  template struct Stack< int >;
}
