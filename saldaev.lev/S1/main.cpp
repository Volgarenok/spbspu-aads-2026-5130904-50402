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
