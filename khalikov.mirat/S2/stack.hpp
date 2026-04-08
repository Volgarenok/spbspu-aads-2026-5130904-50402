#ifndef STACK_HPP
#define STACK_HPP
#include "list.hpp"
#include <iostream>

namespace khalikov
{
  template < class T >
  class Stack
  {
  private:
    List< T > list;
  public:
		void push(const T& rhs);
		void pop();
		const T& top() const noexcept;
		T drop();
		size_t size() const noexcept;
		bool isEmpty() const noexcept;
	};
}
