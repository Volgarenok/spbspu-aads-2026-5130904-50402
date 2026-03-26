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
    LIter< T > tail_;
  };

  template < class T >
  void Stack< T >::push(const T &val)
  {
    data_.push_front(val);
  }

  template < class T >
  void Stack< T >::pop() noexcept
  {
    data_.pop_front();
  }

  template < class T >
  T &Stack< T >::get() noexcept
  {
    return data_.get_head()->val;
  }

  template < class T >
  bool Stack< T >::empty() const noexcept
  {
    return data_.empty();
  }

  template < class T >
  void Queue< T >::push(const T &val)
  {
    if (!data_.empty()) {
      tail_ = data_.insert_after(tail_, val);
    } else {
			data_.push_front(val);
			tail_ = data_.get_head();
    }
  }

  template < class T >
  void Queue< T >::pop() noexcept
  {
		data_.pop_front();
		if(data_.begin() == data_.end()) {
			tail_ = LIter< T >();
		}
  }

  template < class T >
  T &Queue< T >::get() noexcept
  {
    return data_.get_head()->val;
  }

  template < class T >
  bool Queue< T >::empty() const noexcept
  {
    return data_.empty();
  }
}
