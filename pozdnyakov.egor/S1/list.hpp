#ifndef POZDNYAKOV_LIST_HPP
#define POZDNYAKOV_LIST_HPP

#include <cstddef>
#include <iterator>
#include <utility>

namespace pozdnyakov
{

  namespace detail
  {
    template < class T > struct Node
    {
      T data;
      Node *next;
      Node(const T &val, Node *n = nullptr):
        data(val),
        next(n)
      {}
    };
  }

  template < class T > class List;
  template < class T > class LCIter;

  template < class T > class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  private:
    detail::Node< T > *ptr;
    explicit LIter(detail::Node< T > *p):
      ptr(p)
    {}

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    LIter():
      ptr(nullptr)
    {}

    reference operator*() const
    {
      return ptr->data;
    }
    pointer operator->() const
    {
      return &(ptr->data);
    }

    LIter &operator++()
    {
      if (ptr)
        ptr = ptr->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter &other) const
    {
      return ptr == other.ptr;
    }
    bool operator!=(const LIter &other) const
    {
      return ptr != other.ptr;
    }
  };

  template < class T > class LCIter
  {
    friend class List< T >;

  private:
    const detail::Node< T > *ptr;
    explicit LCIter(const detail::Node< T > *p):
      ptr(p)
    {}

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    LCIter():
      ptr(nullptr)
    {}
    LCIter(const LIter< T > &other):
      ptr(other.ptr)
    {}

    reference operator*() const
    {
      return ptr->data;
    }
    pointer operator->() const
    {
      return &(ptr->data);
    }

    LCIter &operator++()
    {
      if (ptr)
        ptr = ptr->next;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LCIter &other) const
    {
      return ptr == other.ptr;
    }
    bool operator!=(const LCIter &other) const
    {
      return ptr != other.ptr;
    }
  };

  template < class T > class List
  {
  private:
    detail::Node< T > *head;

  public:
    List() noexcept:
      head(nullptr)
    {}

    ~List() noexcept
    {
      clear();
    }

    List(const List &other):
      head(nullptr)
    {
      if (other.empty())
        return;

      try {
        auto it = other.cbegin();
        pushFront(*it);
        LIter< T > tail = begin();
        ++it;

        while (it != other.cend()) {
          insertAfter(tail, *it);
          ++tail;
          ++it;
        }
      } catch (...) {
        clear();
        throw;
      }
    }

    List(List &&other) noexcept:
      head(other.head)
    {
      other.head = nullptr;
    }

    List &operator=(const List &other)
    {
      if (this != &other) {
        List tmp(other);
        std::swap(head, tmp.head);
      }
      return *this;
    }

    List &operator=(List &&other) noexcept
    {
      if (this != &other) {
        clear();
        head = other.head;
        other.head = nullptr;
      }
      return *this;
    }

    void pushFront(const T &val)
    {
      head = new detail::Node< T >(val, head);
    }

    void popFront() noexcept
    {
      if (head) {
        detail::Node< T > *temp = head;
        head = head->next;
        delete temp;
      }
    }

    void insertAfter(LIter< T > pos, const T &val)
    {
      if (pos.ptr) {
        pos.ptr->next = new detail::Node< T >(val, pos.ptr->next);
      }
    }

    void eraseAfter(LIter< T > pos) noexcept
    {
      if (pos.ptr && pos.ptr->next) {
        detail::Node< T > *temp = pos.ptr->next;
        pos.ptr->next = temp->next;
        delete temp;
      }
    }

    void clear() noexcept
    {
      while (head) {
        popFront();
      }
    }

    bool empty() const noexcept
    {
      return head == nullptr;
    }

    T &front()
    {
      return head->data;
    }
    const T &front() const
    {
      return head->data;
    }

    LIter< T > begin()
    {
      return LIter< T >(head);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    LCIter< T > cbegin() const
    {
      return LCIter< T >(head);
    }
    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }
  };

}

#endif
