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
    return !(data_.size());
  }

  template < class T >
  void Queue< T >::push(const T &value)
  {
    data_.pushBack(value);
  }

  template < class T >
  T &Queue< T >::front()
  {
    if (!empty()) {
      auto h = data_.begin();
      return *h;
    }
    throw std::logic_error("Cannot read from empty queue");
  }

  template < class T >
  const T &Queue< T >::front() const
  {
    if (!empty()) {
      auto h = data_.begin();
      return *h;
    }
    throw std::logic_error("Cannot read from empty queue");
  }

  template < class T >
  void Queue< T >::pop()
  {
    if (empty()) {
      throw std::logic_error("Cannot pop from empty queue");
    }
    data_.popFront();
  }

  template < class T >
  void Queue< T >::clear() noexcept
  {
    data_.clear();
  }

  template < class T >
  Queue< T > &Queue< T >::operator=(Queue &&other)
  {
    if (&other != this) {
      data_.swap(other.data_);
      other.data_.clear();
    }
    return *this;
  }

  template < class T >
  size_t Queue< T >::size() const noexcept
  {
    return data_.size();
  }

  template struct Queue< int >;
  template struct Queue< std::string >;
  template struct Queue< saldaev::Queue< std::string > >;
}
