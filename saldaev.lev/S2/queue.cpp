#include "queue.hpp"
#include <stdexcept>

namespace saldaev
{
  template < class T >
  Queue< T >::Queue():
    data_()
  {}

  template < class T >
  bool Queue< T >::empty() const noexcept
  {
    return !(data_.getLength());
  }

  template < class T >
  void Queue< T >::push(const T &value)
  {
    data_.newTail(value);
  }

  template < class T >
  T &Queue< T >::front()
  {
    if (!empty()) {
      LIter h = data_.begin();
      return h.getData();
    }
    throw std::logic_error("Cannot read from empty queue");
  }

  template < class T >
  void Queue< T >::pop() noexcept
  {
    data_.cutHead();
  }

  template < class T >
  void Queue< T >::clear() noexcept
  {
    data_.clear();
  }

  template struct Queue< int >;
}
