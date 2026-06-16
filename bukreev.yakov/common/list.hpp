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
    LIter& operator++() noexcept;
    LIter operator++(int) noexcept;
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
    LCIter& operator++() noexcept;
    LCIter operator++(int) noexcept;
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
    size_t count(const T& value) const noexcept;
    LIter< T > begin() const noexcept;
    LIter< T > end() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
    void pushBack(const T& value);
    T popBack() noexcept;
    T popFront() noexcept;
    bool remove (const T& value);

  private:
    Node< T >* m_head;
    Node< T >* m_tail;
  };

  template< class T >
  List< T >::List() noexcept:
    m_head(nullptr),
    m_tail(nullptr)
  {
  }

  template< class T >
  List< T >::List(const List< T >& other):
    m_head(nullptr),
    m_tail(nullptr)
  {
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
    Node< T >* cur = m_head;
    while (cur)
    {
      Node< T >* n = cur->next;
      delete cur;
      cur = n;
    }

    m_head = nullptr;
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
  size_t List< T >::count(const T& value) const noexcept
  {
    size_t res = 0;
    for (LCIter< T > it = cbegin(); it != cend(); it++)
    {
      if (*it == value)
      {
        res++;
      }
    }

    return res;
  }

  template< class T >
  LIter< T > List< T >::begin() const noexcept
  {
    return LIter< T >(m_head);
  }

  template< class T >
  LIter< T > List< T >::end() const noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >(m_head);
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
      m_head = node;
    }
    m_tail = node;
  }

  template< class T >
  T List< T >::popBack() noexcept
  {
    Node< T >* penult = m_head;
    Node< T >* last = m_head;
    while (last->next)
    {
      penult = last;
      last = last->next;
    }

    T retval = last->val;
    delete last;

    if (m_head == m_tail)
    {
      m_head = m_tail = nullptr;
    }
    else
    {
      penult->next = nullptr;
      m_tail = penult;
    }

    return retval;
  }

  template< class T >
  T List< T >::popFront() noexcept
  {
    T retval = m_head->val;

    Node< T >* next = m_head->next;
    delete m_head;
    m_head = next;

    if (m_head == nullptr)
    {
      m_tail = nullptr;
    }

    return retval;
  }

  template< class T >
  bool List< T >::remove (const T& value)
  {
    Node< T >* cur = m_head;
    Node< T >* prev = m_head;
    bool deleted = false;

    while (cur)
    {
      if (cur->val == value)
      {
        prev->next = cur->next;
        delete cur;
        deleted = true;

        if (cur == m_tail)
        {
          m_tail = prev;
        }

        if (m_tail == m_head)
        {
          m_head = m_tail = nullptr;
        }

        cur = prev->next;
      }
      else
      {
        prev = cur;
        cur = cur->next;
      }
    }

    return deleted;
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
  LIter< T >& LIter< T >::operator++() noexcept
  {
    *this = next();
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int) noexcept
  {
    LIter< T >& temp = *this;
    ++*this;
    return temp;
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
  LCIter< T >& LCIter< T >::operator++() noexcept
  {
    *this = next();
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int) noexcept
  {
    LCIter< T >& temp = *this;
    ++*this;
    return temp;
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
