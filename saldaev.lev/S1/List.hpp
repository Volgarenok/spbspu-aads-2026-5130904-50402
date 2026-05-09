#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>

namespace saldaev
{
  template < class T >
  struct List
  {
  private:
    struct Node;

  public:
    struct LCIter
    {
      friend class List;

      LCIter &operator++() noexcept;
      LCIter operator++(int) noexcept;
      LCIter &operator--() noexcept;
      LCIter operator--(int) noexcept;
      bool operator==(const LCIter &other) const noexcept;
      bool operator!=(const LCIter &other) const noexcept;

      const T &operator*() const;
      const T *operator->() const;

    private:
      typename List::Node *curr;

      explicit LCIter(typename List::Node *node);
    };

    struct LIter
    {
      friend class List;

      LIter &operator++() noexcept;
      LIter operator++(int) noexcept;
      LIter &operator--() noexcept;
      LIter operator--(int) noexcept;

      bool operator==(const LIter &other) const noexcept;
      bool operator!=(const LIter &other) const noexcept;

      T &operator*();
      T *operator->();
      const T &operator*() const;
      const T *operator->() const;

    private:
      typename List::Node *curr;

      explicit LIter(typename List::Node *node);
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

    void insertBefore(LIter it, const T &d);
    void insertAfter(LIter it, const T &d);
    LIter erase(LIter it);

    LIter begin();
    LIter end();
    LCIter begin() const;
    LCIter end() const;

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
