#include <cstddef>

template < class T >
struct LCIter;
template < class T >
struct LIter;
template < class T >
struct List
{
  friend LCIter;
  friend LIter;

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

  bool hasNext() const noexcept;
  bool hasPrev() const noexcept;
  LCIter< T > &operator++();
  LCIter< T > operator++(int);
  LCIter< T > &operator--();
  LCIter< T > operator--(int);
  bool operator==(const LCIter &other) const;
  bool operator!=(const LCIter &other) const;
  bool isBegin() const noexcept;
  bool isEnd() const noexcept;
  const T &getData() const noexcept;

private:
  explicit LCIter(typename List< T >::Node *node);
  typename List< T >::Node *curr;
};

template < class T >
struct LIter
{
  friend class List< T >;

  bool hasNext() const noexcept;
  bool hasPrev() const noexcept;
  LIter< T > &operator++();
  LIter< T > operator++(int);
  LIter< T > &operator--();
  LIter< T > operator--(int);
  bool operator==(const LIter &other) const;
  bool operator!=(const LIter &other) const;
  bool isBegin() const noexcept;
  bool isEnd() const noexcept;
  const T &getData() const noexcept;

  void setData(const T &d);
  void addBefore(const T &d);
  void addAfter(const T &d);
  void cutCurrent() noexcept;

private:
  explicit LIter(typename List< T >::Node *node, List< T > *list);
  typename List< T >::Node *curr;
  List< T > *list;
};

int main()
{}

template < class T >
List< T >::Node::Node(const T &d, Node *n, Node *p):
  data(d),
  next(n),
  prev(p)
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

template < class T >
void List< T >::newHead(const T &d)
{
  Node *n = new Node(d, head->next, head);
  head->next->prev = n;
  head->next = n;
  ++length;
}

template < class T >
void List< T >::newTail(const T &d)
{
  Node *n = new Node(d, tail, tail->prev);
  tail->prev->next = n;
  tail->prev = n;
  ++length;
}

template < class T >
void List< T >::cutHead() noexcept
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
void List< T >::cutTail() noexcept
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
void List< T >::clear() noexcept
{
  while (length > 0) {
    cutHead();
  }
}

template < class T >
LIter< T > List< T >::begin()
{
  return LIter< T >(head);
}

template < class T >
LIter< T > List< T >::end()
{
  return LIter< T >(tail);
}

template < class T >
LCIter< T > List< T >::begin() const
{
  return LCIter< T >(head);
}

template < class T >
LCIter< T > List< T >::end() const
{
  return LCIter< T >(tail);
}

template < class T >
size_t List< T >::getLength() const noexcept
{
  return length;
}

//

template < class T >
LCIter< T >::LCIter(typename List< T >::Node *node):
  curr(node)
{}

template < class T >
bool LCIter< T >::hasNext() const noexcept
{
  return curr->next;
}

template < class T >
bool LCIter< T >::hasPrev() const noexcept
{
  return curr->prev;
}

template < class T >
LCIter< T > &LCIter< T >::operator++()
{
  curr = curr->next;
  return *this;
}

template < class T >
LCIter< T > LCIter< T >::operator++(int)
{
  LCIter< T > old = *this;
  curr = curr->next;
  return old;
}

template < class T >
LCIter< T > &LCIter< T >::operator--()
{
  curr = curr->prev;
  return *this;
}

template < class T >
LCIter< T > LCIter< T >::operator--(int)
{
  LCIter< T > old = *this;
  curr = curr->prev;
  return old;
}

template < class T >
bool LCIter< T >::operator==(const LCIter &other) const
{
  return curr == other.curr;
}

template < class T >
bool LCIter< T >::operator!=(const LCIter &other) const
{
  return !(*this == other);
}

template < class T >
bool LCIter< T >::isBegin() const noexcept
{
  return !hasPrev();
}

template < class T >
bool LCIter< T >::isEnd() const noexcept
{
  return !hasNext();
}

template < class T >
const T &LCIter< T >::getData() const noexcept
{
  return curr->data;
}

//

template < class T >
LIter< T >::LIter(typename List< T >::Node *node, List< T > *list):
  curr(node),
  list(list)
{}

template < class T >
bool LIter< T >::hasNext() const noexcept
{
  return curr->next;
}

template < class T >
bool LIter< T >::hasPrev() const noexcept
{
  return curr->prev;
}

template < class T >
LIter< T > &LIter< T >::operator++()
{
  curr = curr->next;
  return *this;
}

template < class T >
LIter< T > LIter< T >::operator++(int)
{
  LIter< T > old = *this;
  curr = curr->next;
  return old;
}

template < class T >
LIter< T > &LIter< T >::operator--()
{
  curr = curr->prev;
  return *this;
}

template < class T >
LIter< T > LIter< T >::operator--(int)
{
  LIter< T > old = *this;
  curr = curr->prev;
  return old;
}

template < class T >
bool LIter< T >::operator==(const LIter &other) const
{
  return curr == other.curr;
}

template < class T >
bool LIter< T >::operator!=(const LIter &other) const
{
  return !(*this == other);
}

template < class T >
bool LIter< T >::isBegin() const noexcept
{
  return !hasPrev();
}

template < class T >
bool LIter< T >::isEnd() const noexcept
{
  return !hasNext();
}

template < class T >
const T &LIter< T >::getData() const noexcept
{
  return curr->data;
}

template < class T >
void LIter< T >::setData(const T &d)
{
  curr->data = d;
}

template < class T >
void LIter< T >::addBefore(const T &d)
{
  typename List< T >::Node *n = new typename List< T >::Node(d, curr, curr->prev);
  curr->prev->next = n;
  curr->prev = n;
}

template < class T >
void LIter< T >::addAfter(const T &d)
{
  typename List< T >::Node *n = new typename List< T >::Node(d, curr->next, curr);
  curr->next->prev = n;
  curr->next = n;
}

template < class T >
void LIter< T >::cutCurrent() noexcept
{
  curr->next->prev = curr->prev;
  curr->prev->next = curr->next;
  delete curr;
}
