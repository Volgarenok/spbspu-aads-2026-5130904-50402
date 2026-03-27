#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include "iters.hpp"
#include <cstddef>
#include <utility>

namespace strelnikov
{
  template < class T > class List
  {
  public:
    using list = List< T >;
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;
    using node = Node< T >;
    List();
    List(const list &);
    List(list &&) noexcept;
    list &operator=(const list &);
    list &operator=(list &&) noexcept;
    ~List();

    void push_front(const T &);
    void push_front(T &&);
    void pop_front();
    void clear();
    void swap(list &);
    bool empty() const noexcept;
    node *get_head();

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
    node *head_;
  };

  template < class T >
  List< T >::List():
    head_(nullptr)
  {}

  template < class T >
  List< T >::List(const list &other):
    head_(nullptr)
  {
    if (other.head_ == nullptr) {
      return;
    }

    head_ = new node();
    head_->val = other.head_->val;
    head_->next = nullptr;

    node *curr_this = head_;
    node *curr_other = other.head_->next;

    while (curr_other) {
      node *tmp = new node();
      tmp->val = curr_other->val;
      tmp->next = nullptr;

      curr_this->next = tmp;

      curr_this = curr_this->next;
      curr_other = curr_other->next;
    }
  }

  template < class T >
  List< T >::List(list &&other) noexcept:
    head_(other.head_)
  {
    other.head_ = nullptr;
  }

  template < class T > typename List< T >::list &List< T >::operator=(const list &other)
  {
    if (this == &other) {
      return *this;
    }

    list tmp(other);
    swap(tmp);
    return *this;
  }

  template < class T > typename List< T >::list &List< T >::operator=(list &&other) noexcept
  {
    if (this == &other) {
      return *this;
    }

    swap(other);
    other.clear();
    return *this;
  }

  template < class T > List< T >::~List()
  {
    clear();
  }

  template < class T > void List< T >::push_front(const T &value)
  {
    node *new_head = new node();
    new_head->next = head_;
    new_head->val = value;

    head_ = new_head;
  }

  template < class T > void List< T >::push_front(T &&value)
  {
    node *new_head = new node();
    new_head->next = head_;
    new_head->val = std::move(value);

    head_ = new_head;
  }

  template < class T > void List< T >::pop_front()
  {
    node *tmp_head = head_->next;
    delete head_;
    head_ = tmp_head;
  }

  template < class T > void List< T >::clear()
  {
    while (head_) {
      pop_front();
    }
  }

  template < class T > void List< T >::swap(list &other)
  {
    std::swap(head_, other.head_);
  }

  template < class T > bool List< T >::empty() const noexcept
  {
    return head_ == nullptr;
  }

  template < class T > typename List< T >::iterator List< T >::insert_after(const_iterator pos, const T &value)
  {
    node *curr = const_cast< node * >(pos.curr_);
    node *new_node = new node();
    new_node->val = value;

    new_node->next = curr->next;
    curr->next = new_node;

    return iterator(new_node);
  }

  template < class T > typename List< T >::iterator List< T >::insert_after(const_iterator pos, T &&value)
  {
    node *curr = const_cast< node * >(pos.curr_);
    node *new_node = new node();
    new_node->val = std::move(value);

    new_node->next = curr->next;
    curr->next = new_node;

    return iterator(new_node);
  }

  template < class T >
  typename List< T >::iterator List< T >::insert_after(const_iterator pos, size_t s, const T &value)
  {
    iterator curr = pos;
    for (size_t i = 0; i < s; ++i) {
      curr = insert_after(curr, value);
    }
    return curr;
  }

  template < class T > typename List< T >::iterator List< T >::erase_after(const_iterator pos)
  {
    node *curr = const_cast< node * >(pos.curr_);
    node *to_delete = curr->next;

    if (!to_delete) {
      return iterator(nullptr);
    }

    curr->next = to_delete->next;

    delete to_delete;

    return iterator(curr->next);
  }

  template < class T > typename List< T >::iterator List< T >::begin() noexcept
  {
    return iterator(head_);
  }

  template < class T > typename List< T >::const_iterator List< T >::begin() const noexcept
  {
    return const_iterator(head_);
  }

  template < class T > typename List< T >::const_iterator List< T >::cbegin() const noexcept
  {
    return const_iterator(head_);
  }

  template < class T > typename List< T >::iterator List< T >::end() noexcept
  {
    return iterator(nullptr);
  }

  template < class T > typename List< T >::const_iterator List< T >::end() const noexcept
  {
    return const_iterator(nullptr);
  }

  template < class T > typename List< T >::const_iterator List< T >::cend() const noexcept
  {
    return const_iterator(nullptr);
  }
  template < class T > typename List< T >::node *List< T >::get_head()
  {
    return head_;
  }
}

#endif
