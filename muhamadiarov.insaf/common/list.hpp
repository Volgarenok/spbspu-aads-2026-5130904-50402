#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "liter.hpp"
#include "lciter.hpp"
#include <iostream>

namespace muhamadiarov
{
  template <class T>
  class List
  {
  public:
    List() noexcept;
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List& operator=(const List< T >& other);
    List& operator=(List< T >&& other) noexcept;
    ~List() noexcept;
    LIter< T > begin();
    LIter< T > end();
    LCIter< T > cbegin() const;
    LCIter< T > cend() const;
    void pushFront(const T& value);
    void pushBack(const T& value);
    void popFront() noexcept;
    void popBack() noexcept;
    void clear() noexcept;
    size_t size() const noexcept;
  private:
    Node< T >* head_;
    size_t size_;
  };
}

namespace muh = muhamadiarov;

template <class T>
muh::List< T >::List() noexcept:
  head_(nullptr),
  size_(0)
{}

template <class T>
muh::List< T >::List(const List< T >& other):
  head_(nullptr),
  size_(0)
{
  if (!other.head_)
  {
    return;
  }
  try
  {
    head_ = new Node< T >{other.head_->data_, nullptr, nullptr};
    Node< T >* current = head_;
    Node< T >* currentOther = other.head_->next_;
    ++size_;
    while (currentOther != other.head_)
    {
      current->next_ = new Node< T >{currentOther->data_, nullptr, current};
      current = current->next_;
      currentOther = currentOther->next_;
      ++size_;
    }
    current->next_ = head_;
    head_->prev_ = current;
  }
  catch(...)
  {
    clear();
    throw;
  }
}

template <class T>
muh::List< T >::List(List< T >&& other) noexcept:
  head_(other.head_),
  size_(other.size_)
{
  other.head_ = nullptr;
  other.size_ = 0;
}

template <class T>
muh::List< T >& muh::List< T >::operator=(const List< T >& other)
{
  if (this != &other)
  {
    List< T > time(other);
    std::swap(head_, time.head_);
    std::swap(size_, time.size_);
  }
  return *this;
}

template <class T>
muh::List< T >& muh::List< T >::operator=(List< T >&& other) noexcept
{
  if (this != &other)
  {
    clear();
    this->head_ = other.head_;
    this->size_ = other.size_;
    other.head_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template <class T>
muh::List< T >::~List() noexcept
{
  clear();
}

template <class T>
void muh::List< T >::clear() noexcept
{
  if (!head_)
  {
    return;
  }
  Node< T >* current = head_->next_;
  while (current != head_)
  {
    Node< T >* next = current->next_;
    delete current;
    current = next;
  }
  delete head_;
  head_ = nullptr;
  size_ = 0;
}

template <class T>
void muh::List< T >::pushFront(const T& value)
{
  Node< T >* newNode = new Node< T >{value, nullptr, nullptr};
  try
  {
    if (!head_)
    {
      head_ = newNode;
      head_->next_ = head_;
      head_->prev_ = head_;
    }
    else
    {
      newNode->next_ = head_;
      newNode->prev_ = head_->prev_;
      head_->prev_->next_ = newNode;
      head_->prev_ = newNode;
      head_ = newNode;
    }
    ++size_;
  }
  catch(...)
  {
    delete newNode;
    throw;
  }
}

template <class T>
void muh::List< T >::pushBack(const T& value)
{
  Node< T >* newNode = new Node< T >{value, nullptr, nullptr};
  try
  {
    if (!head_)
    {
      head_ = newNode;
      head_->next_ = head_;
      head_->prev_ = head_;
    }
    else
    {
      newNode->next_ = head_;
      newNode->prev_ = head_->prev_;
      head_->prev_->next_ = newNode;
      head_->prev_ = newNode;
    }
    ++size_;
  }
  catch(...)
  {
    delete newNode;
    throw;
  }
}

template <class T>
void muh::List< T >::popFront() noexcept
{
  if (!head_)
  {
    return;
  }

  if (size_ == 1)
  {
    delete head_;
    head_ = nullptr;
  }
  else
  {
    Node< T >* newHead = head_->next_;
    head_->prev_->next_ = newHead;
    newHead->prev_ = head_->prev_;
    delete head_;
    head_ = newHead;
  }
  --size_;
}

template <class T>
void muh::List< T >::popBack() noexcept
{
  if (!head_)
  {
    return;
  }

  if (size_ == 1)
  {
    delete head_;
    head_ = nullptr;
  }
  else
  {
    Node< T >* last = head_->prev_;
    head_->prev_ = last->prev_;
    last->prev_->next_ = head_;
    delete last;
  }
  --size_;
}

template <class T>
size_t muh::List< T >::size() const noexcept
{
  return size_;
}

template <class T>
muh::LIter< T > muh::List< T >::begin()
{
  return {head_};
}

template <class T>
muh::LIter< T > muh::List< T >::end()
{
  if (size_ == 0)
  {
    return nullptr;
  }
  return {head_->prev_};
}

template <class T>
muh::LCIter< T > muh::List< T >::cbegin() const
{
  return {head_};
}

template <class T>
muh::LCIter< T > muh::List< T >::cend() const
{
  if (size_ == 0)
  {
    return nullptr;
  }
  return {head_->prev_};
}
#endif
