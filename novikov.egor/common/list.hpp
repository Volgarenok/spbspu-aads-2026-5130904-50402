#ifndef NOVIKOV_LIST_HPP
#define NOVIKOV_LIST_HPP

#include <cassert>
#include <memory>

namespace novikov
{
  template < class T > struct Node
  {
    T data;
    Node< T > *next;
  };

  template < class T > class LIter
  {
    Node< T > *node;

  public:
    explicit LIter(Node< T > *n = nullptr):
      node(n)
    {}
    T &operator*()
    {
      assert(node);
      return node->data;
    }
    T *operator->()
    {
      assert(node);
      return std::addressof(node->data);
    }
    LIter &operator++()
    {
      node = node->next;
      return *this;
    }
    LIter operator++(int)
    {
      LIter tmp(*this);
      ++(*this);
      return tmp;
    }
    bool operator==(const LIter &rhs) const
    {
      return node == rhs.node;
    }
    bool operator!=(const LIter &rhs) const
    {
      return node != rhs.node;
    }
  };

  template < class T > class LCIter
  {
    const Node< T > *node;

  public:
    explicit LCIter(const Node< T > *n = nullptr):
      node(n)
    {}
    const T &operator*() const
    {
      assert(node);
      return node->data;
    }
    const T *operator->() const
    {
      assert(node);
      return std::addressof(node->data);
    }
    LCIter &operator++()
    {
      node = node->next;
      return *this;
    }
    LCIter operator++(int)
    {
      LCIter tmp(*this);
      ++(*this);
      return tmp;
    }
    bool operator==(const LCIter &rhs) const
    {
      return node == rhs.node;
    }
    bool operator!=(const LCIter &rhs) const
    {
      return node != rhs.node;
    }
  };

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
      while (last->next != head) {
        last = last->next;
      }
      last->next = node;
      node->next = head;
    }
    void push_front(const T &val)
    {
    Node<T> *node = new Node<T>{val, nullptr};
    if (!head) {
        head = node;
        node->next = head;
        return;
    }
    Node<T> *last = head;
    while (last->next != head) {
        last = last->next;
    }
    node->next = head;
    last->next = node;
    head = node;
    }
    void pop_front()
    {
      if (!head)
        return;
      if (head->next == head) {
        delete head;
        head = nullptr;
        return;
      }
      Node< T > *last = head;
      while (last->next != head) {
        last = last->next;
      }
      Node< T > *tmp = head;
      head = head->next;
      last->next = head;
      delete tmp;
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
}
#endif
