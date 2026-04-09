#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"
#include <iostream>

namespace khalikov
{
  template < class T >
  class Queue
  {
  private:
    List< T > list;
  public:
		const T& front() const;
		const T& back() const;
		void pop();
		void push(const T& rhs);
		T drop();
		size_t size() const noexcept;
		bool isEmpty() const noexcept;
		void swap(Queue& rhs) noexcept;
	};
}


#endif
