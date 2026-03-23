#ifndef BILIST_HPP
#define BILIST_HPP
#include <algorithm>
#include <stdexcept>

namespace shirokov
{
  template < class T >
  class BiList;

  template < class T >
  class BLIter
  {
  public:
    explicit BLIter(typename BiList< T >::Node*);
    bool operator==(const BLIter< T >&) const noexcept;
    bool operator!=(const BLIter< T >&) const noexcept;
    BLIter< T >& operator++();
    BLIter< T >& operator--();
    T& operator*();

  private:
    friend class BiList< T >;
    typename BiList< T >::Node* curr = nullptr;
  };

  template < class T >
  class BLCIter
  {
  public:
    explicit BLCIter(const typename BiList< T >::Node*);
    bool operator==(const BLCIter< T >&);
    bool operator!=(const BLCIter< T >&);
    BLCIter< T >& operator++();
    BLCIter< T >& operator--();
    const T& operator*();

  private:
    friend class BiList< T >;
    const typename BiList< T >::Node* curr = nullptr;
  };

  template < class T >
  class BiList
  {
  public:
    struct Node
    {
      T value;
      Node* next = nullptr;
      Node* prev = nullptr;
    };
    BiList() = default;

    BiList(const BiList< T >&);
    BiList(BiList< T >&&);

    ~BiList();

    BiList< T >& operator=(const BiList< T >&);
    BiList< T >& operator=(BiList< T >&&);

    BLIter< T > begin();
    BLCIter< T > cbegin() const;

    BLIter< T > end();
    BLCIter< T > cend() const;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void pushFront(const T&);
    void pushFront(T&&);
    void pushBack(const T&);
    void pushBack(T&&);

    void popFront();
    void popBack();

    void clear() noexcept;
    bool empty() const noexcept;

  private:
    Node* head = nullptr;
    Node* tail = nullptr;
  };
}

template < class T >
bool shirokov::BiList< T >::empty() const noexcept
{
  return !head;
}

template < class T >
shirokov::BiList< T >::BiList::~BiList()
{
  clear();
}

template < class T >
void shirokov::BiList< T >::clear() noexcept
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
shirokov::BLIter< T > shirokov::BiList< T >::begin()
{
  return shirokov::BLIter< T >(head);
}

template < class T >
shirokov::BLIter< T > shirokov::BiList< T >::end()
{
  return shirokov::BLIter< T >(nullptr);
}

template < class T >
shirokov::BLCIter< T > shirokov::BiList< T >::cend() const
{
  return shirokov::BLCIter< T >(nullptr);
}

template < class T >
bool shirokov::BLIter< T >::operator==(const shirokov::BLIter< T >& other) const noexcept
{
  return curr == other.curr;
}

template < class T >
shirokov::BLIter< T >::BLIter(typename BiList< T >::Node* node):
  curr(node)
{}

template < class T >
void shirokov::BiList< T >::pushBack(T&& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{std::move(value)}; // T::T(T&&)
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
      next = new Node{std::move(value), nullptr, tail}; // T::T(T&&)
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
void shirokov::BiList< T >::pushBack(const T& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{value}; // T::T(const T&)
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
      next = new Node{value, nullptr, tail}; // T::T(const T&)
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
T& shirokov::BiList< T >::front()
{
  if (!head)
  {
    throw std::out_of_range("Empty list");
  }
  return head->value;
}

template < class T >
const T& shirokov::BiList< T >::front() const
{
  if (!head)
  {
    throw std::out_of_range("Empty list");
  }
  return head->value;
}

template < class T >
T& shirokov::BiList< T >::back()
{
  if (!head)
  {
    throw std::out_of_range("Empty list");
  }
  return tail->value;
}

template < class T >
const T& shirokov::BiList< T >::back() const
{
  if (!head)
  {
    throw std::out_of_range("Empty list");
  }
  return tail->value;
}

template < class T >
T& shirokov::BLIter< T >::operator*()
{
  if (!curr)
  {
    throw std::logic_error("Dereferencing null iterator");
  }
  return curr->value;
}

template < class T >
shirokov::BLIter< T >& shirokov::BLIter< T >::operator++()
{
  if (!curr)
  {
    throw std::logic_error("Incrementing invalid iterator");
  }
  curr = curr->next;
  return *this;
}

template < class T >
shirokov::BLIter< T >& shirokov::BLIter< T >::operator--()
{
  if (!curr)
  {
    throw std::logic_error("Decrementing invalid iterator");
  }
  curr = curr->prev;
  return *this;
}

template < class T >
void shirokov::BiList< T >::pushFront(const T& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{value}; // T::T(const T&)
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
      prev = new Node{value, head, nullptr}; // T::T(const T&)
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
void shirokov::BiList< T >::pushFront(T&& value)
{
  if (!head)
  {
    head = nullptr;
    try
    {
      head = new Node{std::move(value)}; // T::T(T&&)
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
      prev = new Node{std::move(value), head, nullptr}; // T::T(T&&)
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
void shirokov::BiList< T >::popFront()
{
  if (!head)
  {
    throw std::out_of_range("List is empty");
  }
  Node* newHead = head->next;
  delete head;
  head = newHead;
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
void shirokov::BiList< T >::popBack()
{
  if (!tail)
  {
    throw std::out_of_range("List is empty");
  }
  Node* newTail = tail->prev;
  delete tail;
  tail = newTail;
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
shirokov::BLCIter< T > shirokov::BiList< T >::cbegin() const
{
  return shirokov::BLCIter< T >(head);
}

template < class T >
const T& shirokov::BLCIter< T >::operator*()
{
  if (!curr)
  {
    throw std::logic_error("Dereferencing null iterator");
  }
  return curr->value;
}

template < class T >
shirokov::BLCIter< T >& shirokov::BLCIter< T >::operator++()
{
  if (!curr)
  {
    throw std::logic_error("Incrementing invalid iterator");
  }
  curr = curr->next;
  return *this;
}

template < class T >
shirokov::BLCIter< T >& shirokov::BLCIter< T >::operator--()
{
  if (!curr)
  {
    throw std::logic_error("Decrementing invalid iterator");
  }
  curr = curr->prev;
  return *this;
}

template < class T >
shirokov::BLCIter< T >::BLCIter(const typename BiList< T >::Node* node):
  curr(node)
{}

template < class T >
shirokov::BiList< T >::BiList(const BiList< T >& other)
{
  try
  {
    Node* curr = nullptr;
    Node* otherCurr = other.head;
    while (otherCurr)
    {
      if (!curr)
      {
        head = new Node{otherCurr->value}; // T::T(const T&)
        curr = head;
        otherCurr = otherCurr->next;
        continue;
      }
      Node* next = new Node{otherCurr->value, nullptr, curr}; // T::T(const T&)
      curr->next = next;
      curr = next;
      otherCurr = otherCurr->next;
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
shirokov::BiList< T >& shirokov::BiList< T >::operator=(const BiList< T >& other)
{
  if (this == &other)
  {
    return *this;
  }
  try
  {
    Node* curr = nullptr;
    Node* otherCurr = other.head;
    while (otherCurr)
    {
      if (!curr)
      {
        head = new Node{otherCurr->value}; // T::T(const T&)
        curr = head;
        otherCurr = otherCurr->next;
        continue;
      }
      Node* next = new Node{otherCurr->value, nullptr, curr}; // T::T(const T&)
      curr->next = next;
      curr = next;
      otherCurr = otherCurr->next;
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
shirokov::BiList< T >::BiList(BiList< T >&& other):
  head(other.head),
  tail(other.tail)
{
  other.head = nullptr;
  other.tail = nullptr;
}

template < class T >
shirokov::BiList< T >& shirokov::BiList< T >::operator=(BiList< T >&& other)
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
bool shirokov::BLIter< T >::operator!=(const BLIter< T >& other) const noexcept
{
  return !(*this == other);
}

template < class T >
bool shirokov::BLCIter< T >::operator==(const BLCIter< T >& other)
{
  return curr == other.curr;
}

template < class T >
bool shirokov::BLCIter< T >::operator!=(const BLCIter< T >& other)
{
  return !(*this == other);
}

#endif
