#ifndef BILIST_HPP
#define BILIST_HPP
#include <algorithm>
#include <stdexcept>

namespace shirokov
{
  template< class T >
  class BiList;

  template< class T >
  class BLIter
  {
  public:
    BLIter(typename BiList< T >::Node*, BiList< T >*);
    bool operator==(const BLIter< T >&) const noexcept;
    bool operator!=(const BLIter< T >&) const noexcept;
    BLIter< T >& operator++();
    BLIter< T >& operator--();
    T& operator*();

  private:
    friend class BiList< T >;
    typename BiList< T >::Node* curr_ = nullptr;
    BiList< T >* list_;
  };

  template< class T >
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
    const typename BiList< T >::Node* curr_ = nullptr;
  };

  template< class T >
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
    BLCIter< T > begin() const;
    BLCIter< T > cbegin() const;

    BLIter< T > end();
    BLCIter< T > end() const;
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

    BLIter< T > insert(BLIter< T > pos, const T& value);
    BLIter< T > erase(BLIter< T > pos);

    void clear() noexcept;
    bool empty() const noexcept;

  private:
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
  };
}

template< class T >
shirokov::BLIter< T > shirokov::BiList< T >::erase(BLIter< T > pos)
{
  if (!pos.curr_)
  {
    return pos;
  }
  BiList< T >::Node *next, *prev;
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
  return BLIter< T >(next, pos.list_);
}

template< class T >
shirokov::BLIter< T > shirokov::BiList< T >::insert(shirokov::BLIter< T > pos, const T& value)
{
  if (!pos.curr_)
  {
    return pos;
  }
  BiList< T >::Node* curr = new Node{value, pos.curr_, pos.curr_->prev};
  shirokov::BLIter< T > newElem(curr, pos.list_);
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
shirokov::BLCIter< T > shirokov::BiList< T >::begin() const
{
  return cbegin();
}

template< class T >
shirokov::BLCIter< T > shirokov::BiList< T >::end() const
{
  return cend();
}

template< class T >
bool shirokov::BiList< T >::empty() const noexcept
{
  return !head_;
}

template< class T >
shirokov::BiList< T >::BiList::~BiList()
{
  clear();
}

template< class T >
void shirokov::BiList< T >::clear() noexcept
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
shirokov::BLIter< T > shirokov::BiList< T >::begin()
{
  return shirokov::BLIter< T >(head_, this);
}

template< class T >
shirokov::BLIter< T > shirokov::BiList< T >::end()
{
  return shirokov::BLIter< T >(nullptr, this);
}

template< class T >
shirokov::BLCIter< T > shirokov::BiList< T >::cend() const
{
  return shirokov::BLCIter< T >(nullptr);
}

template< class T >
bool shirokov::BLIter< T >::operator==(const shirokov::BLIter< T >& other) const noexcept
{
  return curr_ == other.curr_;
}

template< class T >
shirokov::BLIter< T >::BLIter(typename BiList< T >::Node* node, BiList< T >* list):
  curr_(node),
  list_(list)
{}

template< class T >
void shirokov::BiList< T >::pushBack(T&& value)
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
void shirokov::BiList< T >::pushBack(const T& value)
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
T& shirokov::BiList< T >::front()
{
  return head_->value;
}

template< class T >
const T& shirokov::BiList< T >::front() const
{
  return head_->value;
}

template< class T >
T& shirokov::BiList< T >::back()
{
  return tail_->value;
}

template< class T >
const T& shirokov::BiList< T >::back() const
{
  return tail_->value;
}

template< class T >
T& shirokov::BLIter< T >::operator*()
{
  if (!curr_)
  {
    throw std::logic_error("Dereferencing null iterator");
  }
  return curr_->value;
}

template< class T >
shirokov::BLIter< T >& shirokov::BLIter< T >::operator++()
{
  if (!curr_)
  {
    throw std::logic_error("Incrementing invalid iterator");
  }
  curr_ = curr_->next;
  return *this;
}

template< class T >
shirokov::BLIter< T >& shirokov::BLIter< T >::operator--()
{
  if (!curr_)
  {
    throw std::logic_error("Decrementing invalid iterator");
  }
  curr_ = curr_->prev;
  return *this;
}

template< class T >
void shirokov::BiList< T >::pushFront(const T& value)
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
void shirokov::BiList< T >::pushFront(T&& value)
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
void shirokov::BiList< T >::popFront()
{
  if (!head_)
  {
    throw std::out_of_range("List is empty");
  }
  Node* newHead = head_->next;
  delete head_;
  head_ = newHead;
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
void shirokov::BiList< T >::popBack()
{
  if (!tail_)
  {
    throw std::out_of_range("List is empty");
  }
  Node* newTail = tail_->prev;
  delete tail_;
  tail_ = newTail;
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
shirokov::BLCIter< T > shirokov::BiList< T >::cbegin() const
{
  return shirokov::BLCIter< T >(head_);
}

template< class T >
const T& shirokov::BLCIter< T >::operator*()
{
  if (!curr_)
  {
    throw std::logic_error("Dereferencing null iterator");
  }
  return curr_->value;
}

template< class T >
shirokov::BLCIter< T >& shirokov::BLCIter< T >::operator++()
{
  if (!curr_)
  {
    throw std::logic_error("Incrementing invalid iterator");
  }
  curr_ = curr_->next;
  return *this;
}

template< class T >
shirokov::BLCIter< T >& shirokov::BLCIter< T >::operator--()
{
  if (!curr_)
  {
    throw std::logic_error("Decrementing invalid iterator");
  }
  curr_ = curr_->prev;
  return *this;
}

template< class T >
shirokov::BLCIter< T >::BLCIter(const typename BiList< T >::Node* node):
  curr_(node)
{}

template< class T >
shirokov::BiList< T >::BiList(const BiList< T >& other)
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
shirokov::BiList< T >& shirokov::BiList< T >::operator=(const BiList< T >& other)
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
shirokov::BiList< T >::BiList(BiList< T >&& other):
  head_(other.head_),
  tail_(other.tail_)
{
  other.head_ = nullptr;
  other.tail_ = nullptr;
}

template< class T >
shirokov::BiList< T >& shirokov::BiList< T >::operator=(BiList< T >&& other)
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
bool shirokov::BLIter< T >::operator!=(const BLIter< T >& other) const noexcept
{
  return !(*this == other);
}

template< class T >
bool shirokov::BLCIter< T >::operator==(const BLCIter< T >& other)
{
  return curr_ == other.curr_;
}

template< class T >
bool shirokov::BLCIter< T >::operator!=(const BLCIter< T >& other)
{
  return !(*this == other);
}

#endif
