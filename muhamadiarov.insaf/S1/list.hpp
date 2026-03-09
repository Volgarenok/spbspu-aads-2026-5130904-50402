#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include <iostream>

namespace muhamadiarov
{
  template <class T>
  class List
  {
  public:
    List() noexcept;
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List& operator=(const List< T >& other);
    List& operator=(List< T >&& other) noexcept;
    ~List() noexcept;

    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void clear() noexcept;
    size_t size() const noexcept;
  private:
    Node< T >* head_;
    size_t count_;
  };
};
#endif
