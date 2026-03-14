#ifndef POZDNYAKOV_LIST_HPP
#define POZDNYAKOV_LIST_HPP

#include <cstddef>
#include <iterator>
#include <utility>

namespace pozdnyakov
{

  namespace detail
  {
    struct BaseNode
    {
      BaseNode *next;
      BaseNode():
        next(nullptr)
      {}
    };

    template < class T > struct Node: BaseNode
    {
      T data;
      Node(const T &val):
        BaseNode(),
        data(val)
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
    detail::BaseNode *ptr;
    explicit LIter(detail::BaseNode *p):
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
      return static_cast< detail::Node< T > * >(ptr)->data;
    }
    pointer operator->() const
    {
      return &(static_cast< detail::Node< T > * >(ptr)->data);
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
    const detail::BaseNode *ptr;
    explicit LCIter(const detail::BaseNode *p):
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
      return static_cast< const detail::Node< T > * >(ptr)->data;
    }
    pointer operator->() const
    {
      return &(static_cast< const detail::Node< T > * >(ptr)->data);
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
    detail::BaseNode *fakeNode;

  public:
    List()
    {
      fakeNode = new detail::BaseNode();
      fakeNode->next = fakeNode;
    }

    ~List() noexcept
    {
      clear();
      delete fakeNode;
    }

    List(const List &other)
    {
      fakeNode = new detail::BaseNode();
      fakeNode->next = fakeNode;
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
        delete fakeNode;
        throw;
      }
    }

    List(List &&other) noexcept:
      fakeNode(other.fakeNode)
    {
      other.fakeNode = new detail::BaseNode();
      other.fakeNode->next = other.fakeNode;
    }

    List &operator=(const List &other)
    {
      if (this != &other) {
        List tmp(other);
        std::swap(fakeNode, tmp.fakeNode);
      }
      return *this;
    }

    List &operator=(List &&other) noexcept
    {
      if (this != &other) {
        clear();
        delete fakeNode;
        fakeNode = other.fakeNode;
        other.fakeNode = new detail::BaseNode();
        other.fakeNode->next = other.fakeNode;
      }
      return *this;
    }

    void pushFront(const T &val)
    {
      detail::Node< T > *newNode = new detail::Node< T >(val);
      newNode->next = fakeNode->next;
      fakeNode->next = newNode;
    }

    void popFront() noexcept
    {
      if (!empty()) {
        detail::BaseNode *temp = fakeNode->next;
        fakeNode->next = temp->next;
        delete static_cast< detail::Node< T > * >(temp);
      }
    }

    void insertAfter(LIter< T > pos, const T &val)
    {
      if (pos.ptr) {
        detail::Node< T > *newNode = new detail::Node< T >(val);
        newNode->next = pos.ptr->next;
        pos.ptr->next = newNode;
      }
    }

    void eraseAfter(LIter< T > pos) noexcept
    {
      if (pos.ptr && pos.ptr->next != fakeNode) {
        detail::BaseNode *temp = pos.ptr->next;
        pos.ptr->next = temp->next;
        delete static_cast< detail::Node< T > * >(temp);
      }
    }

    void clear() noexcept
    {
      while (!empty()) {
        popFront();
      }
    }

    bool empty() const noexcept
    {
      return fakeNode->next == fakeNode;
    }

    T &front()
    {
      return static_cast< detail::Node< T > * >(fakeNode->next)->data;
    }
    const T &front() const
    {
      return static_cast< const detail::Node< T > * >(fakeNode->next)->data;
    }

    LIter< T > begin()
    {
      return LIter< T >(fakeNode->next);
    }
    LIter< T > end()
    {
      return LIter< T >(fakeNode);
    }
    LCIter< T > cbegin() const
    {
      return LCIter< T >(fakeNode->next);
    }
    LCIter< T > cend() const
    {
      return LCIter< T >(fakeNode);
    }
  };

}

#endif
