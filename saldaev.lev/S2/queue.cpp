#include "queue.hpp"

namespace saldaev
{
  template < class T >
  Queue< T >::Queue():
    data_()
  {}

  template < class T >
  bool Queue< T >::empty() const noexcept
  {
    return false;
  }

  template < class T >
  void Queue< T >::push(const T &value)
  {}

  template < class T >
  T &Queue< T >::front()
  {
    auto dummy = T();
    return dummy;
  }

  template struct Queue< int >;
}
