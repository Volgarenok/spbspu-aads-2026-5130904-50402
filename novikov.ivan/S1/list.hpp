#ifndef LIST_HPP
#define LIST_HPP

namespace novikov
{
  template < class T >
  class List;

  template < class T >
  class LIter
  {
  public:
    explicit LIter(typename List< T >::Node*);
    bool operator==(const LIter< T >&) const noexcept;
    bool operator!=(const LIter< T >&) const noexcept;
    LIter< T >& operator++();
    LIter< T >& operator--();
    T& operator*();

  private:
    friend class List< T >;
    typename List< T >::Node* curr = nullptr;
  };

  template < class T >
  class LCIter
  {
  public:
    explicit LCIter(const typename List< T >::Node*);
    bool operator==(const LCIter< T >&);
    bool operator!=(const LCIter< T >&);
    LCIter< T >& operator++();
    LCIter< T >& operator--();
    const T& operator*();

  private:
    friend class List< T >;
    const typename List< T >::Node* curr = nullptr;
  };

  template < class T >
  class List
  {
  public:
    struct Node
    {
      T value;
      Node* next = nullptr;
      Node* prev = nullptr;
    };
    List() = default;

    List(const List< T >&);
    List(List< T >&&);

    ~List();

    List< T >& operator=(const List< T >&);
    List< T >& operator=(List< T >&&);

    LIter< T > begin();
    LCIter< T > cbegin() const;

    LIter< T > end();
    LCIter< T > cend() const;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void push_front(const T&);
    void push_front(T&&);
    void push_back(const T&);
    void push_back(T&&);

    void pop_front();
    void pop_back();

    void clear() noexcept;
    bool empty() const noexcept;

  private:
    Node* head = nullptr;
    Node* tail = nullptr;
  };
}

template < class T >
bool novikov::List< T >::empty() const noexcept
{
  return !head;
}

template < class T >
novikov::List< T >::~List()
{
  clear();
}

template < class T >
void novikov::List< T >::clear() noexcept
{
  while (head)
  {
    Node* next = head->next;
    delete head;
    head = next;
  }
  head = nullptr;
  tail = nullptr;
}

template < class T >
novikov::LIter< T > novikov::List< T >::begin()
{
  return novikov::LIter< T >(head);
}

template < class T >
novikov::LIter< T > novikov::List< T >::end()
{
  return novikov::LIter< T >(nullptr);
}

template < class T >
novikov::LCIter< T > novikov::List< T >::cbegin() const
{
  return novikov::LCIter< T >(head);
}

template < class T >
novikov::LCIter< T > novikov::List< T >::cend() const
{
  return novikov::LCIter< T >(nullptr);
}

template < class T >
bool novikov::LIter< T >::operator==(const novikov::LIter< T >& other) const noexcept
{
  return curr == other.curr;
}

template < class T >
novikov::LIter< T >::LIter(typename List< T >::Node* node): curr(node)
{
}

#endif
