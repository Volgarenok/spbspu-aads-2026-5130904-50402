#ifndef LIST_HPP
#define LIST_HPP
#include <algorithm>

namespace alisov
{
  template < class T > class BiList;

  template < class T > class BLIter
  {
    explicit BLIter(typename BiList< T >::Node *);
    bool operator==(const BLIter< T > &other) const noexcept;
    bool operator!=(const BLIter< T > &other) const noexcept;
    BLIter< T > operator++();
    BLIter< T > operator--();
    T &operator*();

  private:
    friend class BiList< T >;
    const typename BiList< T >::Node *curr = nullptr;
  };

  template < class T > class BLCIter
  {
    explicit BLCIter(const typename BiList< T >::Node *);
    bool operator==(const BLCIter< T > &other);
    bool operator!=(const BLCIter< T > &other);
    BLCIter< T > operator++();
    BLCIter< T > operator--();
    const T &operator*();

  private:
    friend class BiList< T >;
    const typename BiList< T >::Node *curr = nullptr;
  };

  template < class T > class BiList
  {
  public:
    struct Node
    {
      T value;
      Node *next = nullptr;
      Node *prev = nullptr;
    };
    BiList() = default;

    BiList(const BiList< T > &);
    BiList(BiList< T > &&);

    ~BiList() noexcept;

    BiList< T > &operator=(const BiList< T > &);
    BiList< T > &operator=(BiList< T > &&);

    BLIter< T > begin();
    BLCIter< T > begin() const;
    BLCIter< T > cbegin() const noexcept;

    BLIter< T > end();
    BLCIter< T > end() const;
    BLCIter< T > cend() const noexcept;

    T &front();
    const T &front() const;
    T &back();
    const T &back() const;

    void push_front(const T &);
    void push_front(T &&);
    void push_back(const T &);
    void push_back(T &&);

    void pop_front();
    void pop_back();

    void clear() noexcept;
    bool empty() const noexcept;

  private:
    Node *head = nullptr;
    Node *tail = nullptr;
  };
}

template < class T > alisov::BLIter< T >::BLIter(typename BiList< T >::Node *node) : curr(node)
{
}

template < class T > bool alisov::BiList< T >::empty() const noexcept
{
  return !head;
}

template < class T > alisov::BiList< T >::~BiList() noexcept
{
  clear();
}

template < class T > void alisov::BiList< T >::clear() noexcept
{
  while (head)
  {
    Node *next = head->next;
    delete head;
    head = next;
  }
  head = nullptr;
  tail = nullptr;
}

template < class T > alisov::BLIter< T > alisov::BiList< T >::begin()
{
  return alisov::BLIter< T >{head};
}

template < class T > alisov::BLIter< T > alisov::BiList< T >::end()
{
  return alisov::BLIter< T >{nullptr};
}

template < class T >
bool alisov::BLIter< T >::operator==(const alisov::BLIter< T > &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool alisov::BLIter< T >::operator!=(const alisov::BLIter< T > &other) const noexcept
{
  return !(*this == other);
}

template < class T > alisov::BLIter< T > alisov::BLIter< T >::operator++()
{
  if (!curr)
  {
    throw std::out_of_range("Out of range");
  }
  curr = curr->next;
  return *this;
}

template < class T > alisov::BLIter< T > alisov::BLIter< T >::operator--()
{
  if (!curr)
  {
    throw std::out_of_range("Out of range");
  }
  curr = curr->prev;
  return *this;
}

template < class T > T const &alisov::BLCIter< T >::operator*()
{
  return curr->value;
}

template < class T > void alisov::BiList< T >::push_back(T &&value)
{
  if (!head)
  {
    head = new Node(std::move(value));
    tail = head;
  }
  else
  {
    Node *next = new Node{value, nullptr, tail};
    tail->next->prev = tail;
    tail = tail->next;
  }
}

template < class T > void alisov::BiList< T >::push_back(const T &value)
{
  if (!head)
  {
    head = new Node{value, nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node *next = new Node{value, nullptr, tail};
    tail->next = next;
    tail = next;
  }
}

template < class T > void alisov::BiList< T >::push_front(T &&value)
{
  if (!head)
  {
    head = new Node{value, nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node *prev = new Node{std::move(value), head, nullptr};
    head->prev = prev;
    head = prev;
  }
}

template < class T > void alisov::BiList< T >::push_front(const T &value)
{
  if (!head)
  {
    head = new Node{value, nullptr, nullptr};
    tail = head;
  }
  else
  {
    Node *prev = new Node{std::move(value), head, nullptr};
    head->prev = prev;
    head = prev;
  }
}

template < class T > T &alisov::BiList< T >::front()
{
  return head->value;
}

template < class T > const T &alisov::BiList< T >::front() const
{
  return head->value;
}

template < class T > T &alisov::BiList< T >::back()
{
  return tail->value
}

template < class T > const T &alisov::BiList< T >::back() const
{
  return tail->value
}

#endif