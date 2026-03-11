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

    void pushFront(const T& value);
    void pushBack(const T& value);
    void popFront() noexcept;
    void popBack() noexcept;
    void clear() noexcept;
    size_t size() const noexcept;
  private:
    Node< T >* head_;
    size_t size_;
  };
}
#endif
