#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include <cstddef>

namespace dirko
{
  template < class T > class List
  {
  public:
    List();
    List(const List< T > &);
    List(List< T > &&) noexcept;
    List &operator=(const List< T > &);
    List &operator=(List< T > &&) noexcept;
    ~List();
    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const noexcept;
    void swap(List< T > &) noexcept;

  private:
    Node< T > *fake_;
    size_t size_;
  };

}

#endif
