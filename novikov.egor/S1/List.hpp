#ifndef NOVIKOV_LIST_HPP
#define NOVIKOV_LIST_HPP

#include "Node.hpp"
#include "LIter.hpp"
#include "LCIter.hpp"

namespace novikov
{
  template < class T > class List
  {
    Node< T > *head;

  public:
    List():
      head(nullptr)
    {}

    ~List()
    {
      clear();
    }

    List(const List &other):
      head(nullptr)
    {
      if (!other.head)
        return;

      Node< T > *cur = other.head;

      do {
        push_back(cur->data);
        cur = cur->next;
      } while (cur != other.head);
    }

    List &operator=(List other)
    {
      swap(other);
      return *this;
    }

    void swap(List &other)
    {
      Node< T > *tmp = head;
      head = other.head;
      other.head = tmp;
    }

    bool empty() const
    {
      return head == nullptr;
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

    void push_back(const T &val)
    {
      Node< T > *node = new Node< T >{val, nullptr};

      if (!head) {
        head = node;
        node->next = head;
        return;
      }

      Node< T > *last = head;

      while (last->next != head)
        last = last->next;

      last->next = node;
      node->next = head;
    }

    void push_front(const T &val)
    {
      push_back(val);
      head = head->next;
    }

    void clear()
    {
      if (!head)
        return;

      Node< T > *cur = head->next;

      while (cur != head) {
        Node< T > *tmp = cur;
        cur = cur->next;
        delete tmp;
      }

      delete head;
      head = nullptr;
    }
  };

  template < class T > struct IterState
  {
    LIter< T > current;
    LIter< T > start;
  };
}

#endif
