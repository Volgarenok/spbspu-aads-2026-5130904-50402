#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include "node.hpp"
#include "iters.hpp"

namespace dirko
{
  template < class T >
  class List
  {
  public:
    List();
    List(const List< T > &);
    List(List< T > &&) noexcept;
    List &operator=(const List< T > &);
    List &operator=(List< T > &&) noexcept;
    ~List();
    Iter< T > begin();
    Iter< T > end();
    CIter< T > cbegin() const;
    CIter< T > cend() const;
    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const noexcept;
    void swap(List< T > &) noexcept;

  private:
    Node< T > *fake_;
    Node< T > *tail_;
    size_t size_;
  };

}

#endif
