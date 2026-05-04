#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>

namespace saldaev
{
  template < class T >
  struct List
  {
    struct LCIter
    {
      friend class List;

      LCIter< T > &operator++() noexcept;
      LCIter< T > operator++(int) noexcept;
      LCIter< T > &operator--() noexcept;
      LCIter< T > operator--(int) noexcept;
      bool operator==(const LCIter &other) const noexcept;
      bool operator!=(const LCIter &other) const noexcept;

      const T &operator*() const;
      const T *operator->() const;

    private:
      typename List< T >::Node *curr;

      explicit LCIter(typename List< T >::Node *node);
    };

    struct LIter
    {
      friend class List;

      LIter< T > &operator++() noexcept;
      LIter< T > operator++(int) noexcept;
      LIter< T > &operator--() noexcept;
      LIter< T > operator--(int) noexcept;

      bool operator==(const LIter &other) const noexcept;
      bool operator!=(const LIter &other) const noexcept;

      T &operator*();
      T *operator->();
      const T &operator*() const;
      const T *operator->() const;

    private:
      typename List< T >::Node *curr;

      explicit LIter(typename List< T >::Node *node);
    };

    List();
    ~List();
    List(const List &other);
    List &operator=(const List &other);
    List(List &&other);
    List &operator=(List &&other);

    void pushFront(const T &d);
    void pushBack(const T &d);
    T popFront();
    T popBack();
    void clear() noexcept;
    void swap(List &other) noexcept;

    void insertBefore(LIter< T > it, const T &d);
    void insertAfter(LIter< T > it, const T &d);
    LIter< T > erase(LIter< T > it) noexcept;

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;

    size_t size() const noexcept;

  private:
    struct Node
    {
      T data;
      Node *next;
      Node *prev;

      Node(const T &d, Node *n, Node *p);
    };

    Node *head;
    Node *tail;
    size_t length;
  };
}

#endif
