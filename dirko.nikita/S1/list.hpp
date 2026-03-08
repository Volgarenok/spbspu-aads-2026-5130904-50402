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
    List(const List< T > &other);
    List(List< T > &&other) noexcept;
    List &operator=(const List< T > &other);
    List &operator=(List< T > &&other) noexcept;
    ~List();
    void push_front(const T &val);
    void push_back(const T &val);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const noexcept;
  private:
    Node< T > *fake_;
    size_t size_;
  };

}

#endif
