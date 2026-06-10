#ifndef LIST_HPP
#define LIST_HPP
#include <algorithm>
#include <stdexcept>

namespace novikov
{
  template< class T >
  class List;

  template< class T >
  class LIter
  {
  public:
    LIter(typename List< T >::Node*, List< T >*);
    bool operator==(const LIter< T >&) const noexcept;
    bool operator!=(const LIter< T >&) const noexcept;
    LIter< T >& operator++();
    LIter< T >& operator--();
    T& operator*();

  private:
    friend class List< T >;
    typename List< T >::Node* curr_ = nullptr;
    List< T >* list_;
  };

  template< class T >
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
    const typename List< T >::Node* curr_ = nullptr;
  };

  template< class T >
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
    LCIter< T > begin() const;
    LCIter< T > cbegin() const;

    LIter< T > end();
    LCIter< T > end() const;
    LCIter< T > cend() const;

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

    LIter< T > insert(LIter< T > pos, const T& value);
    LIter< T > erase(LIter< T > pos);

    void clear() noexcept;
    bool empty() const noexcept;

  private:
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
  };
}

template< class T >
novikov::LIter< T > novikov::List< T >::insert(novikov::LIter< T > pos, const T& value)
{
  if (!pos.curr_)
  {
    return pos;
  }
  List< T >::Node* curr = new Node{value, pos.curr_, pos.curr_->prev};
  novikov::LIter< T > newElem(curr, pos.list_);
  if (pos.curr_->prev)
  {
    pos.curr_->prev->next = newElem.curr_;
  }
  else
  {
    pos.list_->head_ = newElem.curr_;
  }
  pos.curr_->prev = newElem.curr_;
  return newElem;
}

template< class T >
novikov::LIter< T > novikov::List< T >::erase(LIter< T > pos)
{
  if (!pos.curr_)
  {
    return pos;
  }
  List< T >::Node *next, *prev;
  prev = pos.curr_->prev;
  next = pos.curr_->next;
  delete pos.curr_;
  if (next)
  {
    next->prev = prev;
    if (!prev)
    {
      pos.list_->head_ = next;
    }
  }
  if (prev)
  {
    prev->next = next;
    if (!next)
    {
      pos.list_->tail_ = prev;
    }
  }
  if (!prev && !next)
  {
    pos.list_->head_ = nullptr;
    pos.list_->tail_ = nullptr;
  }
  return LIter< T >(next, pos.list_);
}

template< class T >
novikov::LCIter< T > novikov::List< T >::begin() const
{
  return cbegin();
}

template< class T >
novikov::LCIter< T > novikov::List< T >::end() const
{
  return cend();
}

template< class T >
bool novikov::List< T >::empty() const noexcept
{
  return !head_;
}

template< class T >
novikov::List< T >::~List()
{
  clear();
}

template< class T >
void novikov::List< T >::clear() noexcept
{
  while (head_)
  {
    Node* next = head_->next;
    delete head_;
    head_ = next;
  }
  head_ = nullptr;
  tail_ = nullptr;
}

template< class T >
novikov::LIter< T > novikov::List< T >::begin()
{
  return novikov::LIter< T >(head_, this);
}

template< class T >
novikov::LIter< T > novikov::List< T >::end()
{
  return novikov::LIter< T >(nullptr, this);
}

template< class T >
novikov::LCIter< T > novikov::List< T >::cend() const
{
  return novikov::LCIter< T >(nullptr);
}

template< class T >
bool novikov::LIter< T >::operator==(const novikov::LIter< T >& other) const noexcept
{
  return curr_ == other.curr_;
}

template< class T >
novikov::LIter< T >::LIter(typename List< T >::Node* node, List< T >* list):
  curr_(node),
  list_(list)
{}

template< class T >
void novikov::List< T >::pushBack(T&& value)
{
  if (!head_)
  {
    head_ = nullptr;
    try
    {
      head_ = new Node{std::move(value)};
      tail_ = head_;
    }
    catch (...)
    {
      delete head_;
      throw;
    }
  }
  else
  {
    Node* next = nullptr;
    try
    {
      next = new Node{std::move(value), nullptr, tail_};
      tail_->next = next;
      tail_ = next;
    }
    catch (...)
    {
      delete next;
      throw;
    }
  }
}

template< class T >
void novikov::List< T >::pushBack(const T& value)
{
  if (!head_)
  {
    head_ = nullptr;
    try
    {
      head_ = new Node{value};
      tail_ = head_;
    }
    catch (...)
    {
      delete head_;
      throw;
    }
  }
  else
  {
    Node* next = nullptr;
    try
    {
      next = new Node{value, nullptr, tail_};
      tail_->next = next;
      tail_ = next;
    }
    catch (...)
    {
      delete next;
      throw;
    }
  }
}

template< class T >
T& novikov::List< T >::front()
{
  return head_->value;
}

template< class T >
const T& novikov::List< T >::front() const
{
  return head_->value;
}

template< class T >
T& novikov::List< T >::back()
{
  return tail_->value;
}

template< class T >
const T& novikov::List< T >::back() const
{
  return tail_->value;
}

template< class T >
T& novikov::LIter< T >::operator*()
{
  if (!curr_)
  {
    throw std::logic_error("The iterator is invalid");
  }
  return curr_->value;
}

template< class T >
novikov::LIter< T >& novikov::LIter< T >::operator++()
{
  if (!curr_)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr_ = curr_->next;
  return *this;
}

template< class T >
novikov::LIter< T >& novikov::LIter< T >::operator--()
{
  if (!curr_)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr_ = curr_->prev;
  return *this;
}

template< class T >
void novikov::List< T >::pushFront(const T& value)
{
  if (!head_)
  {
    head_ = nullptr;
    try
    {
      head_ = new Node{value};
      tail_ = head_;
    }
    catch (...)
    {
      delete head_;
      throw;
    }
  }
  else
  {
    Node* prev = nullptr;
    try
    {
      prev = new Node{value, head_, nullptr};
      head_->prev = prev;
      head_ = prev;
    }
    catch (...)
    {
      delete prev;
      throw;
    }
  }
}

template< class T >
void novikov::List< T >::pushFront(T&& value)
{
  if (!head_)
  {
    head_ = nullptr;
    try
    {
      head_ = new Node{std::move(value)};
      tail_ = head_;
    }
    catch (...)
    {
      delete head_;
      throw;
    }
  }
  else
  {
    Node* prev = nullptr;
    try
    {
      prev = new Node{std::move(value), head_, nullptr};
      head_->prev = prev;
      head_ = prev;
    }
    catch (...)
    {
      delete prev;
      throw;
    }
  }
}

template< class T >
void novikov::List< T >::popFront()
{
  if (!head_)
  {
    throw std::out_of_range("The list is empty");
  }
  Node* new_head = head_->next;
  delete head_;
  head_ = new_head;
  if (head_)
  {
    head_->prev = nullptr;
  }
  else
  {
    tail_ = nullptr;
  }
}

template< class T >
void novikov::List< T >::popBack()
{
  if (!tail_)
  {
    throw std::out_of_range("The list is empty");
  }
  Node* new_tail = tail_->prev;
  delete tail_;
  tail_ = new_tail;
  if (tail_)
  {
    tail_->next = nullptr;
  }
  else
  {
    head_ = nullptr;
  }
}

template< class T >
novikov::LCIter< T > novikov::List< T >::cbegin() const
{
  return novikov::LCIter< T >(head_);
}

template< class T >
const T& novikov::LCIter< T >::operator*()
{
  if (!curr_)
  {
    throw std::logic_error("The iterator is invalid");
  }
  return curr_->value;
}

template< class T >
novikov::LCIter< T >& novikov::LCIter< T >::operator++()
{
  if (!curr_)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr_ = curr_->next;
  return *this;
}

template< class T >
novikov::LCIter< T >& novikov::LCIter< T >::operator--()
{
  if (!curr_)
  {
    throw std::logic_error("The iterator is invalid");
  }
  curr_ = curr_->prev;
  return *this;
}

template< class T >
novikov::LCIter< T >::LCIter(const typename List< T >::Node* node):
  curr_(node)
{}

template< class T >
novikov::List< T >::List(const List< T >& other)
{
  try
  {
    Node* curr = nullptr;
    Node* otherCurr = other.head_;
    while (otherCurr)
    {
      if (!curr)
      {
        head_ = new Node{otherCurr->value};
        curr = head_;
        otherCurr = otherCurr->next;
        continue;
      }
      Node* next = new Node{otherCurr->value, nullptr, curr};
      curr->next = next;
      curr = next;
      otherCurr = otherCurr->next;
    }
    tail_ = curr;
  }
  catch (...)
  {
    clear();
    throw;
  }
}

template< class T >
novikov::List< T >& novikov::List< T >::operator=(const List< T >& other)
{
  if (this == &other)
  {
    return *this;
  }
  try
  {
    Node* curr = nullptr;
    Node* otherCurr = other.head_;
    while (otherCurr)
    {
      if (!curr)
      {
        head_ = new Node{otherCurr->value};
        curr = head_;
        otherCurr = otherCurr->next;
        continue;
      }
      Node* next = new Node{otherCurr->value, nullptr, curr};
      curr->next = next;
      curr = next;
      otherCurr = otherCurr->next;
    }
    tail_ = curr;
  }
  catch (...)
  {
    clear();
    throw;
  }
  return *this;
}

template< class T >
novikov::List< T >::List(List< T >&& other):
  head_(other.head_),
  tail_(other.tail_)
{
  other.head_ = nullptr;
  other.tail_ = nullptr;
}

template< class T >
novikov::List< T >& novikov::List< T >::operator=(List< T >&& other)
{
  if (this == &other)
  {
    return *this;
  }
  clear();
  head_ = other.head_;
  tail_ = other.tail_;
  other.head_ = nullptr;
  other.tail_ = nullptr;
  return *this;
}

template< class T >
bool novikov::LIter< T >::operator!=(const LIter< T >& other) const noexcept
{
  return !(*this == other);
}

template< class T >
bool novikov::LCIter< T >::operator==(const LCIter< T >& other)
{
  return curr_ == other.curr_;
}

template< class T >
bool novikov::LCIter< T >::operator!=(const LCIter< T >& other)
{
  return !(*this == other);
}

#endif
