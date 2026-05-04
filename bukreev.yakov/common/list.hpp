#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace bukreev
{
  template< class T >
  struct Node
  {
    T val;
    Node< T >* next;
  };

  template< class T > class List;

  template< class T >
  class LIter
  {
    friend class List< T >;

  public:
    LIter() = default;
    LIter(Node< T >* node) noexcept;
    T& operator*() const;
    LIter& operator++(int) noexcept;
    bool operator==(const LIter< T >& other);
    bool operator!=(const LIter< T >& other);

  private:
    LIter next() const noexcept;

  private:
    Node< T >* m_cur;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;

  public:
    LCIter() = default;
    LCIter(Node< T >* node) noexcept;
    T& operator*() const;
    LCIter& operator++(int) noexcept;
    bool operator==(const LCIter< T >& other);
    bool operator!=(const LCIter< T >& other);

  private:
    LCIter next() const noexcept;

  private:
    Node< T >* m_cur;
  };

  template< class T >
  class List
  {
  public:
    List() noexcept;
    List(const List< T >& other);
    ~List() noexcept;
    List< T >& operator=(const List< T >& other);
    void clear() noexcept;
    size_t size() const noexcept;
    LIter< T > begin() const noexcept;
    LIter< T > end() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
    void pushBack(const T& value);
    T popBack() noexcept;

  private:
    Node< T > m_fake;
    Node< T >* m_tail;
  };

  template< class T >
  List< T >::List() noexcept
  {
    m_fake.next = nullptr;
    m_tail = nullptr;
  }

  template< class T >
  List< T >::List(const List< T >& other)
  {
    m_fake.next = nullptr;
    m_tail = nullptr;

    for (LIter< T > it = other.begin(); it != other.end(); it++)
    {
      pushBack(*it);
    }
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    clear();

    for (LIter< T > it = other.begin(); it != other.end(); it++)
    {
      pushBack(*it);
    }

    return *this;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    Node< T >* cur = m_fake.next;
    while (cur)
    {
      Node< T >* n = cur->next;
      delete cur;
      cur = n;
    }

    m_fake.next = nullptr;
    m_tail = nullptr;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    size_t res = 0;
    for (LCIter< T > it = cbegin(); it != cend(); it++)
    {
      res++;
    }

    return res;
  }

  template< class T >
  LIter< T > List< T >::begin() const noexcept
  {
    return LIter< T >(m_fake.next);
  }

  template< class T >
  LIter< T > List< T >::end() const noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(m_fake.next);
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    Node< T >* node = new Node< T >;
    node->val = value;
    node->next = nullptr;

    if (m_tail)
    {
      m_tail->next = node;
    }
    else
    {
      m_fake.next = node;
    }
    m_tail = node;
  }

  template< class T >
  T List< T >::popBack() noexcept
  {
    Node< T >* lastNode;
    Node< T >* prevNode = std::addressof(m_fake);
    for (Node< T >* node = m_fake.next; node!= nullptr; node = node->next)
    {
      lastNode = node;
      if (node->next)
      {
        prevNode = prevNode->next;
      }
    }

    prevNode->next = nullptr;
    m_tail = prevNode;

    T retval = lastNode->val;
    delete lastNode;

    return retval;
  }

  template< class T >
  LIter< T >::LIter(Node< T >* node) noexcept
  {
    m_cur = node;
  }

  template< class T >
  T& LIter< T >::operator*() const
  {
    return m_cur->val;
  }

  template< class T >
  LIter< T >& LIter< T >::operator++(int) noexcept
  {
    *this = next();
    return *this;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter< T >& other)
  {
    return m_cur == other.m_cur;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter< T >& other)
  {
    return m_cur != other.m_cur;
  }

  template< class T >
  LIter< T > LIter< T >::next() const noexcept
  {
    return LIter< T >(m_cur->next);
  }

  template< class T >
  LCIter< T >::LCIter(Node< T >* node) noexcept
  {
    m_cur = node;
  }

  template< class T >
  T& LCIter< T >::operator*() const
  {
    return m_cur->val;
  }

  template< class T >
  LCIter< T >& LCIter< T >::operator++(int) noexcept
  {
    *this = next();
    return *this;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter< T >& other)
  {
    return m_cur == other.m_cur;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter< T >& other)
  {
    return m_cur != other.m_cur;
  }

  template< class T >
  LCIter< T > LCIter< T >::next() const noexcept
  {
    return LCIter< T >(m_cur->next);
  }
}

#endif
