#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include <cstddef>

namespace alisov
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

  private:
    Node< T > *fake_;
    Node< T > *tail_;
    size_t size_;
  };

}

#endif