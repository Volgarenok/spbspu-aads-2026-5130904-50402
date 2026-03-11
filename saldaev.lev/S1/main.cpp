#include <iostream>
#include <string>

namespace saldaev
{
  template < class T >
  struct LCIter;
  template < class T >
  struct LIter;
  template < class T >
  struct List
  {
    friend LCIter< T >;
    friend LIter< T >;

    List();
    ~List();
    List(const List &other);
    List &operator=(const List &other);
    List(List &&other);
    List &operator=(List &&other);

    void newHead(const T &d);
    void newTail(const T &d);
    void cutHead() noexcept;
    void cutTail() noexcept;
    void clear() noexcept;

    void setData(LIter< T > it, const T &d);
    void addBefore(LIter< T > it, const T &d);
    void addAfter(LIter< T > it, const T &d);
    LIter< T > cutCurrent(LIter< T > it) noexcept;

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;

    size_t getLength() const noexcept;

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

  template < class T >
  struct LCIter
  {
    friend class List< T >;

    bool isValid() const noexcept;
    bool hasNext() const noexcept;
    bool hasPrev() const noexcept;
    LCIter< T > &operator++() noexcept;
    LCIter< T > operator++(int) noexcept;
    LCIter< T > &operator--() noexcept;
    LCIter< T > operator--(int) noexcept;
    bool operator==(const LCIter &other) const noexcept;
    bool operator==(const LIter< T > &other) const noexcept;
    bool operator!=(const LCIter &other) const noexcept;
    bool operator!=(const LIter< T > &other) const noexcept;
    const T &getData() const noexcept;

  private:
    typename List< T >::Node *curr;
    bool valid;

    explicit LCIter(typename List< T >::Node *node);
  };

  template < class T >
  struct LIter
  {
    friend class List< T >;

    bool isValid() const noexcept;
    bool hasNext() const noexcept;
    bool hasPrev() const noexcept;
    LIter< T > &operator++() noexcept;
    LIter< T > operator++(int) noexcept;
    LIter< T > &operator--() noexcept;
    LIter< T > operator--(int) noexcept;
    bool operator==(const LIter &other) const noexcept;
    bool operator==(const LCIter &other) const noexcept;
    bool operator!=(const LIter &other) const noexcept;
    bool operator!=(const LCIter &other) const noexcept;
    const T &getData() const noexcept;
    T &getData() noexcept;

  private:
    typename List< T >::Node *curr;
    bool valid;

    explicit LIter(typename List< T >::Node *node);
  };
}

int main()
{}

template < class T >
saldaev::List< T >::List():
  head(nullptr),
  tail(nullptr),
  length(0)
{
  head = new Node(T(), nullptr, nullptr);
  try {
    tail = new Node(T(), head, nullptr);
  } catch (...) {
    delete head;
    throw;
  }
  head->next = tail;
}

template < class T >
saldaev::List< T >::~List()
{
  clear();
  delete head;
  delete tail;
}

template < class T >
saldaev::List< T >::List(const List &other):
  List()
{
  try {
    for (auto it = other.begin(); it != other.end(); ++it) {
      newTail(it.getData());
    }
  } catch (...) {
    clear();
    throw
  }
}

template < class T >
saldaev::List< T > &saldaev::List< T >::operator=(const List< T > &other)
{
  if (this != &other) {
    List< T > tmp(other);
    std::swap(head, tmp.head);
    std::swap(tail, tmp.tail);
    std::swap(length, tmp.length);
  }
  return *this;
}

template < class T >
saldaev::List< T >::List(List &&other):
  List()
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(length, other.length);
}

template < class T >
saldaev::List< T > &saldaev::List< T >::operator=(List< T > &&other)
{
  if (this != &other) {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
  }
  return *this;
}

template < class T >
void saldaev::List< T >::newHead(const T &d)
{
  Node *n = new Node(d, head->next, head);
  head->next->prev = n;
  head->next = n;
  ++length;
}

template < class T >
void saldaev::List< T >::newTail(const T &d)
{
  Node *n = new Node(d, tail, tail->prev);
  tail->prev->next = n;
  tail->prev = n;
  ++length;
}

template < class T >
void saldaev::List< T >::cutHead() noexcept
{
  if (length != 0) {
    Node *tmp = head->next->next;
    delete head->next;
    head->next = tmp;
    tmp->prev = head;
    --length;
  }
}

template < class T >
void saldaev::List< T >::cutTail() noexcept
{
  if (length != 0) {
    Node *tmp = tail->prev->prev;
    delete tail->prev;
    tail->prev = tmp;
    tmp->next = tail;
    --length;
  }
}

template < class T >
void saldaev::List< T >::clear() noexcept
{
  while (length > 0) {
    cutHead();
  }
}

template < class T >
void saldaev::List< T >::setData(LIter< T > it, const T &d)
{
  if (it.isValid()) {
    it.curr->data = d;
  }
}

template < class T >
void saldaev::List< T >::addBefore(LIter< T > it, const T &d)
{
  if (it.isValid()) {
    typename List< T >::Node *n = new typename List< T >::Node(d, it.curr, it.curr->prev);
    it.curr->prev->next = n;
    it.curr->prev = n;
    ++length;
  }
}

template < class T >
void saldaev::List< T >::addAfter(LIter< T > it, const T &d)
{
  if (it.isValid()) {
    typename List< T >::Node *n = new typename List< T >::Node(d, it.curr->next, it.curr);
    it.curr->next->prev = n;
    it.curr->next = n;
    ++length;
  }
}

template < class T >
saldaev::LIter< T > saldaev::List< T >::cutCurrent(LIter< T > it) noexcept
{
  if (it.isValid()) {
    typename List< T >::Node *curr = it.curr;
    typename List< T >::Node *next = curr->next;
    curr->next->prev = curr->prev;
    curr->prev->next = curr->next;
    delete curr;
    --length;
    return LIter< T >(next);
  }
  return end();
}

template < class T >
saldaev::LIter< T > saldaev::List< T >::begin()
{
  return LIter< T >(head->next);
}

template < class T >
saldaev::LIter< T > saldaev::List< T >::end()
{
  return LIter< T >(tail);
}

template < class T >
saldaev::LCIter< T > saldaev::List< T >::begin() const
{
  return LCIter< T >(head->next);
}

template < class T >
saldaev::LCIter< T > saldaev::List< T >::end() const
{
  return LCIter< T >(tail);
}

template < class T >
size_t saldaev::List< T >::getLength() const noexcept
{
  return length;
}
