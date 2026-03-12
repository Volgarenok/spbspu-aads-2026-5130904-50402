#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include <cstddef>

namespace strelnikov
{

  template < class T > class LIter;
  template < class T > class LCIter;
  template < class T > class List
  {
  public:
    using list = List< T >;
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;
    List();
    List(const list &);
    List(list &&) noexcept;
    list operator=(const list &);
    list operator=(list &&) noexcept;
    ~List();

    void push_front(const T &);
    void push_front(T &&);
    void pop_front();
    void clear();
    bool empty();

    iterator insert_after(const_iterator pos, const T& value);
    iterator insert_after(const_iterator pos, T&& value);
    iterator insert_after(const_iterator pos, size_type n, const T& value);

    iterator erase_after(const_iterator pos);
    
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

  private:
    Node< T > *head_;
  };
}

#endif