#include <iostream>
#include <string>

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
  bool operator==(const LIter &other) const noexcept;
  bool operator!=(const LCIter &other) const noexcept;
  bool operator!=(const LIter &other) const noexcept;
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

int main()
{}

template < class T >
List< T >::List():
  head(nullptr),
  tail(nullptr),
  length(0)
{
  head = static_cast< Node * >(::operator new(sizeof(Node)));
  try {
    tail = static_cast< Node * >(::operator new(sizeof(Node)));
  } catch (...) {
    ::operator delete(head);
    throw;
  }
  head->next = tail;
  head->prev = nullptr;
  tail->prev = head;
  tail->next = nullptr;
}

template < class T >
List< T >::~List()
{
  clear();
  ::operator delete(head);
  ::operator delete(tail);
}

template < class T >
List< T >::List(const List &other):
  List()
{
  for (auto it = other.begin(); !it.isEnd(); ++it) {
    newTail(it.getData());
  }
}

template < class T >
List< T > &List< T >::operator=(const List< T > &other)
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
List< T >::List(List &&other):
  List()
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(length, other.length);
}

template < class T >
List< T > &List< T >::operator=(List< T > &&other)
{
  if (this != &other) {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
    other.clear();
  }
  return *this;
}
