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

template < class T >
const T& khalikov::Queue < T >::back() const
{
	if (isEmpty())
	{
		throw std::logic_error("Queue is empty");
	}
	auto it = list.cbegin();
	for (size_t i = 0; i < list.size() - 1; ++i)
	{
		++it;
	}
	return *it;
}

template < class T >
const T& khalikov::Queue < T >::front() const
{
	if (isEmpty())
	{
		throw std::logic_error("Queue is empty");
	}
	return *list.cbegin();
}

template < class T >
void khalikov::Queue < T >::push(const T& rhs)
{
	list.pushBack(rhs);
}

template < class T >
void khalikov::Queue < T >::pop()
{
	if (isEmpty())
	{
		throw std::logic_error("Queue is empty");
	}
	list.popFront();
}

template < class T >
T khalikov::Queue < T >::drop()
{
	T temp = front();
	pop();
	return temp;
}

template < class T >
bool khalikov::Queue < T >::isEmpty() const noexcept
{
	return list.isEmpty();
}

template < class T >
size_t khalikov::Queue < T >::size() const noexcept
{
	return list.size();
}

template < class T >
void khalikov::Queue < T >::swap(Queue& rhs) noexcept
{
	list.swap(rhs.list);
}

#endif
