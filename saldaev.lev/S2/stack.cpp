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
    return !(data_.getLength());
  }

  template < class T >
  void Stack< T >::push(const T &value)
  {
    data_.newHead(value);
  }

  template < class T >
  T &Stack< T >::top()
  {
    if (!empty()) {
      LIter< T > h = data_.begin();
      return h.getData();
    }
    throw std::logic_error("Cannot read from empty stack");
  }

  template < class T >
  const T &Stack< T >::top() const
  {
    if (!empty()) {
      LCIter< T > h = data_.begin();
      return h.getData();
    }
    throw std::logic_error("Cannot read from empty stack");
  }

  template < class T >
  void Stack< T >::pop() noexcept
  {
    data_.cutHead();
  }

  template < class T >
  void Stack< T >::clear() noexcept
  {
    data_.clear();
  }

  template < class T >
  Stack< T > &Stack< T >::operator=(Stack &&other)
  {
    if (&other != this) {
      data_.swap(other.data_);
      other.data_.clear();
    }
    return *this;
  }

  template < class T >
  size_t Stack< T >::size() const noexcept
  {
    return data_.getLength();
  }

  template struct Stack< int >;
  template struct Stack< long long >;
  template struct Stack< std::string >;
}
