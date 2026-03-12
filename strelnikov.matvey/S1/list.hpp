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

    LIter(const LIter &) noexcept;

    LIter(LIter &&) noexcept;

    LIter &operator=(const LIter &);

    LIter &operator=(LIter &&) noexcept;

    ~LIter() noexcept = default;

    T &operator*() const noexcept;
    T *operator->() const noexcept;

    LIter &operator++();
    LIter operator++(int);

    bool operator==(const LIter &) const noexcept;
    bool operator!=(const LIter &) const noexcept;

  private:
    Node< T > *curr_;
  };

  template < class T > class LCIter
  {
    friend class List< T >;

  public:
    LCIter() noexcept;

    LCIter(const LCIter &) noexcept;

    LCIter(LCIter &&) noexcept;

    LCIter &operator=(const LCIter &);

    LCIter &operator=(LCIter &&) noexcept;

    ~LCIter() noexcept = default;

    T &operator*() const noexcept;
    T *operator->() const noexcept;

    LCIter &operator++();
    LCIter operator++(int);

    bool operator==(const LCIter &) const noexcept;
    bool operator!=(const LCIter &) const noexcept;

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
    list &operator=(const list &);
    list operator=(list &&) noexcept;
    ~List();

    void push_front(const T &);
    void push_front(T &&);
    void pop_front();
    void clear();
    bool empty();

    iterator insert_after(const_iterator, const T &);
    iterator insert_after(const_iterator, T &&);
    iterator insert_after(const_iterator, size_t, const T &);

    iterator erase_after(const_iterator);

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

  private:
    Node< T > *head_;
  };

  template < class T >
  List< T >::List():
    head_(nullptr)
  {}

  template < class T >
  List< T >::List(const list &other):
    head_(nullptr)
  {
    Node< T > *curr_other = other.head_;

    while (curr_other) {
      Node< T > *new_node = new Node< T >();

      new_node->val = curr_other->val;
      new_node->next = curr_other->next;

      if (head_) {
        Node< T > *curr_this = head_;
        while (curr_this) {
          curr_this = curr_this->next;
        }
        curr_this->next = new_node;
      } else {
        head_ = new_node;
      }

      curr_other = curr_other->next;
    }
  }

  template < class T >
  List< T >::List(list &&other) noexcept:
    head_(other.head_)
  {
    other.head_ = nullptr;
  }

  template < class T > List< T > &List< T >::operator=(const list &other)
  {
    Node < T > *tmp_head = new Node < T >();
    tmp_head->val = other.head_->val;
    tmp_head->next = other.head_->next;
    
    Node < T > *tmp_head = other.head_;
    
    return 
  }

}

#endif