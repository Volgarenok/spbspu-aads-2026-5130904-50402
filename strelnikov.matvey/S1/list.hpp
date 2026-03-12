#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include <cstddef>

namespace strelnikov
{
  template < class T > class List;
  template < class T > class LIter
  {
    friend class List< T >;

  public:
    LIter() noexcept;

    LIter(const LIter &other) noexcept;

    LIter(LIter &&other) noexcept;

    LIter &operator=(const LIter &rhs);

    LIter &operator=(LIter &&rhs) noexcept;

    ~LIter() noexcept = default;

    T &operator*() const noexcept;
    T *operator->() const noexcept;

    LIter &operator++();
    LIter operator++(int);

    bool operator==(const LIter &other) const noexcept;
    bool operator!=(const LIter &other) const noexcept;

  private:
    Node< T > *curr_;
  };

  template < class T > class LCIter
  {
    friend class List< T >;

  public:
    LCIter() noexcept;

    LCIter(const LCIter &other) noexcept;

    LCIter(LCIter &&other) noexcept;

    LCIter &operator=(const LCIter &rhs);

    LCIter &operator=(LCIter &&rhs) noexcept;

    ~LCIter() noexcept = default;

    T &operator*() const noexcept;
    T *operator->() const noexcept;

    LCIter &operator++();
    LCIter operator++(int);

    bool operator==(const LCIter &other) const noexcept;
    bool operator!=(const LCIter &other) const noexcept;

  private:
    const Node< T > *curr_;
  };

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

    iterator insert_after(const_iterator pos, const T &value);
    iterator insert_after(const_iterator pos, T &&value);
    iterator insert_after(const_iterator pos, size_type n, const T &value);

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