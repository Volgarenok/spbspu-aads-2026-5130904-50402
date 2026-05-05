#ifndef LIST_HPP
#define LIST_HPP
#include <algorithm>
#include <stdexcept>

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

template < class T >
void novikov::List< T >::push_back(T&& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{std::move(value)};
      tail = head;
    }
    catch (...)
    {
      delete head;
      throw;
    }
  }
  else
  {
    Node* next = nullptr;
    try
    {
      next = new Node{std::move(value), nullptr, tail};
      tail->next = next;
      tail = next;
    }
    catch (...)
    {
      delete next;
      throw;
    }
  }
}

template < class T >
void novikov::List< T >::push_back(const T& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{value};
      tail = head;
    }
    catch (...)
    {
      delete head;
      throw;
    }
  }
  else
  {
    Node* next = nullptr;
    try
    {
      next = new Node{value, nullptr, tail};
      tail->next = next;
      tail = next;
    }
    catch (...)
    {
      delete next;
      throw;
    }
  }
}

template < class T >
T& novikov::List< T >::front()
{
  if (!head)
  {
    throw std::out_of_range("The list is empty");
  }
  return head->value;
}

template < class T >
const T& novikov::List< T >::front() const
{
  if (!head)
  {
    throw std::out_of_range("The list is empty");
  }
  return head->value;
}

template < class T >
T& novikov::List< T >::back()
{
  if (!head)
  {
    throw std::out_of_range("The list is empty");
  }
  return tail->value;
}

template < class T >
const T& novikov::List< T >::back() const
{
  if (!head)
  {
    throw std::out_of_range("The list is empty");
  }
  return tail->value;
}

template < class T >
T& novikov::LIter< T >::operator*()
{
  if (!curr)
  {
    throw std::logic_error("The iterator is invalid");
  }
  return curr->value;
}

template < class T >
novikov::LIter< T >& novikov::LIter< T >::operator++()
{
  if (!curr)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr = curr->next;
  return *this;
}

template < class T >
novikov::LIter< T >& novikov::LIter< T >::operator--()
{
  if (!curr)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr = curr->prev;
  return *this;
}

template < class T >
void novikov::List< T >::push_front(const T& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{value};
      tail = head;
    }
    catch (...)
    {
      delete head;
      throw;
    }
  }
  else
  {
    Node* prev = nullptr;
    try
    {
      prev = new Node{value, head, nullptr};
      head->prev = prev;
      head = prev;
    }
    catch (...)
    {
      delete prev;
      throw;
    }
  }
}

template < class T >
void novikov::List< T >::push_front(T&& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{std::move(value)};
      tail = head;
    }
    catch (...)
    {
      delete head;
      throw;
    }
  }
  else
  {
    Node* prev = nullptr;
    try
    {
      prev = new Node{std::move(value), head, nullptr};
      head->prev = prev;
      head = prev;
    }
    catch (...)
    {
      delete prev;
      throw;
    }
  }
}

template < class T >
void novikov::List< T >::pop_front()
{
  if (!head)
  {
    throw std::out_of_range("The list is empty");
  }
  Node* new_head = head->next;
  delete head;
  head = new_head;
  if (head)
  {
    head->prev = nullptr;
  }
  else
  {
    tail = nullptr;
  }
}

template < class T >
void novikov::List< T >::pop_back()
{
  if (!tail)
  {
    throw std::out_of_range("The list is empty");
  }
  Node* new_tail = tail->prev;
  delete tail;
  tail = new_tail;
  if (tail)
  {
    tail->next = nullptr;
  }
  else
  {
    head = nullptr;
  }
}

template < class T >
const T& novikov::LCIter< T >::operator*()
{
  if (!curr)
  {
    throw std::logic_error("The iterator is invalid");
  }
  return curr->value;
}

template < class T >
novikov::LCIter< T >& novikov::LCIter< T >::operator++()
{
  if (!curr)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr = curr->next;
  return *this;
}

template < class T >
novikov::LCIter< T >& novikov::LCIter< T >::operator--()
{
  if (!curr)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr = curr->prev;
  return *this;
}

template < class T >
novikov::LCIter< T >::LCIter(const typename List< T >::Node* node): curr(node)
{
}

template < class T >
novikov::List< T >::List(const List< T >& other)
{
  try
  {
    Node* curr = nullptr;
    Node* other_curr = other.head;
    while (other_curr)
    {
      if (!curr)
      {
        head = new Node{other_curr->value};
        curr = head;
        other_curr = other_curr->next;
        continue;
      }
      Node* next = new Node{other_curr->value, nullptr, curr};
      curr->next = next;
      curr = next;
      other_curr = other_curr->next;
    }
    tail = curr;
  }
  catch (...)
  {
    clear();
    throw;
  }
}

template < class T >
novikov::List< T >& novikov::List< T >::operator=(const List< T >& other)
{
  if (this == &other)
  {
    return *this;
  }
  try
  {
    Node* curr = nullptr;
    Node* other_curr = other.head;
    while (other_curr)
    {
      if (!curr)
      {
        head = new Node{other_curr->value};
        curr = head;
        other_curr = other_curr->next;
        continue;
      }
      Node* next = new Node{other_curr->value, nullptr, curr};
      curr->next = next;
      curr = next;
      other_curr = other_curr->next;
    }
    tail = curr;
  }
  catch (...)
  {
    clear();
    throw;
  }
  return *this;
}

template < class T >
novikov::List< T >::List(List< T >&& other): head(other.head), tail(other.tail)
{
  other.head = nullptr;
  other.tail = nullptr;
}

template < class T >
novikov::List< T >& novikov::List< T >::operator=(List< T >&& other)
{
  if (this == &other)
  {
    return *this;
  }
  clear();
  head = other.head;
  tail = other.tail;
  other.head = nullptr;
  other.tail = nullptr;
  return *this;
}

template < class T >
bool novikov::LIter< T >::operator!=(const LIter< T >& other) const noexcept
{
  return !(*this == other);
}

template < class T >
bool novikov::LCIter< T >::operator==(const LCIter< T >& other)
{
  return curr == other.curr;
}

template < class T >
bool novikov::LCIter< T >::operator!=(const LCIter< T >& other)
{
  return !(*this == other);
}

#endif
