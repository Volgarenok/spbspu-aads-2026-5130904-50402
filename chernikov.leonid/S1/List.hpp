#ifndef LIST_BASE_HPP
#define LIST_BASE_HPP

#include "node.hpp"
#include <cstddef>
#include <stdexcept>

namespace chernikov {

  template < typename T > class LIter;
  template < typename T > class LCIter;

  template < typename T > class List
  {
  private:
    Node< T > *head;
    size_t count;

  public:
    List():
      head(nullptr),
      count(0)
    {
    }
    List(const List &other):
      head(nullptr),
      count(0)
    {
      copy_from(other);
    }
    List(List &&other) noexcept:
      head(other.head),
      count(other.count)
    {
      other.head = nullptr;
      other.count = 0;
    }
    ~List()
    {
      clear();
    }

    bool empty() const
    {
      return head == nullptr;
    }
    size_t size() const
    {
      return count;
    }

    LIter< T > begin()
    {
      return LIter< T >(head);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    LCIter< T > begin() const
    {
      return LCIter< T >(head);
    }
    LCIter< T > end() const
    {
      return LCIter< T >(nullptr);
    }
    LCIter< T > cbegin() const
    {
      return LCIter< T >(head);
    }
    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }

    T &front()
    {
      if (empty())
        throw std::logic_error("List is empty");
      return head->data;
    }

    const T &front() const
    {
      if (empty())
        throw std::logic_error("List is empty");
      return head->data;
    }
    void swap(List &other) noexcept
    {
      std::swap(head, other.head);
      std::swap(count, other.count);
    }
    void add(const T &val);
    LIter< T > insert_after(LIter< T > pos, const T &value);
    void push_back(const T &value);
    void first_delete();
    void clear();
    List< T > &operator=(const List &other);
    List< T > &operator=(List &&other) noexcept;

  private:
    void copy_from(const List &other);
  };

  template < typename T > void List< T >::copy_from(const List &other)
  {
    if (!other.head)
    {
      return;
    }
    head = new Node< T >{other.head->data, nullptr};
    count = 1;
    Node< T > *current = head;
    Node< T > *other_current = other.head->next;
    while (other_current)
    {
      current->next = new Node< T >{other_current->data, nullptr};
      current = current->next;
      other_current = other_current->next;
      ++count;
    }
  }
  template < typename T > void List< T >::add(const T &val)
  {
    head = new Node< T >(val, head);
    ++count;
  }
  template < typename T > LIter< T > List< T >::insert_after(LIter< T > pos, const T &value)
  {
    if (!pos.ptr)
    {
      if (!head)
      {
        add(value);
        return LIter< T >(head);
      }
      Node< T > *current = head;
      while (current->next)
      {
        current = current->next;
      }
      current->next = new Node< T >(value, nullptr);
      ++count;
      return LIter< T >(current->next);
    }

    Node< T > *new_node = new Node< T >(value, pos.ptr->next);
    pos.ptr->next = new_node;
    ++count;
    return LIter< T >(new_node);
  }
  template < typename T > void List< T >::push_back(const T &value)
  {
    Node< T > *new_node = new Node< T >(value, nullptr);
    if (!head)
    {
      head = new_node;
    } else
    {
      Node< T > *current = head;
      while (current->next)
      {
        current = current->next;
      }
      current->next = new_node;
    }
    ++count;
  }
  template < typename T > void List< T >::first_delete()
  {
    if (empty())
    {
      throw std::logic_error("List is empty");
    }
    Node< T > *old = head;
    head = head->next;
    delete old;
    --count;
  }
  template < typename T > void List< T >::clear()
  {
    while (!empty())
    {
      first_delete();
    }
  }
  template < typename T > List< T > &List< T >::operator=(const List &other)
  {
    if (this != &other)
    {
      List tmp(other);
      swap(tmp);
    }
    return *this;
  }
  template < typename T > List< T > &List< T >::operator=(List &&other) noexcept
  {
    if (this != &other)
    {
      clear();
      head = other.head;
      count = other.count;
      other.head = nullptr;
      other.count = 0;
    }
    return *this;
  }
}

#endif
