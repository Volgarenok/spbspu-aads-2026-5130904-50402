template < class T > struct List
{
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

  LCIter begin() const;
  LCIter end() const;
  LIter begin();
  LIter end();

  size_t getLength() const noexcept;

private:
  struct Node
  {
    T data;
    Node *next;
    Node *prev;
  };

  Node *head;
  Node *tail;
  size_t length;
};

template < class T > struct LCIter
{
  friend class List< T >;

  bool hasNext() const noexcept;
  bool hasPrev() const noexcept;
  LIter &operator++();
  LIter operator++(int);
  LIter &operator--();
  LIter operator--(int);
  bool operator==(const LCIter &other) const;
  bool isBegin() const noexcept;
  bool isEnd() const noexcept;
  const T &getData() const noexcept;

private:
  explicit LCIter(typename List< T >::Node *curr);
  typename List< T >::Node *curr;
};

template < class T > struct LIter
{
  friend class List< T >;

  bool hasNext() const noexcept;
  bool hasPrev() const noexcept;
  LIter &operator++();
  LIter operator++(int);
  LIter &operator--();
  LIter operator--(int);
  bool operator==(const LCIter &other) const;
  bool isBegin() const noexcept;
  bool isEnd() const noexcept;
  const T &getData() const noexcept;

  void setData(const T &d);
  void addBefore(const T &d);
  void addAfter(const T &d);
  void cutCurrent() noexcept;

private:
  explicit LIter(typename List< T >::Node *node);
  typename List< T >::Node *curr;
  List< T > *list;
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

template < class T > List< T >::~List()
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

template < class T > List< T > &List< T >::operator=(const List< T > &other)
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

template < class T > List< T > &List< T >::operator=(List< T > &&other)
{
  if (this != &other) {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
    other.clear();
  }
  return *this;
}

template < class T > void List< T >::newHead(const T &d)
{
  Node *n = new Node(d, head->next, head);
  head->next->prev = n;
  head->next = n;
  ++length;
}

template < class T > void List< T >::newTail(const T &d)
{
  Node *n = new Node(d, tail, tail->prev);
  tail->prev->next = n;
  tail->prev = n;
  ++length;
}

template < class T > void List< T >::cutHead() noexcept
{
  if (length != 0) {
    Node *tmp = head->next->next;
    delete head->next;
    head->next = tmp;
    tmp->prev = head;
    --length;
  }
}

template < class T > void List< T >::cutTail() noexcept
{
  if (length != 0) {
    Node *tmp = tail->prev->prev;
    delete tail->prev;
    tail->prev = tmp;
    tmp->next = tail;
    --length;
  }
}

template < class T > void List< T >::clear() noexcept
{
  while (length > 0) {
    cutHead();
  }
}
