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
      BaseNode* next;
      BaseNode() : next(nullptr) {}
      virtual ~BaseNode() = default;
    };

    template < class T > struct Node : BaseNode
    {
      T data;
      explicit Node(const T& val) : BaseNode(), data(val) {}
      explicit Node(T&& val) : BaseNode(), data(std::move(val)) {}
    };
  }

  template < class T > class List;
  template < class T > class LCIter;

  template < class T > class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  private:
    detail::BaseNode* ptr;
    explicit LIter(detail::BaseNode* p) : ptr(p) {}

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    LIter() : ptr(nullptr) {}

    reference operator*() const { return static_cast<detail::Node< T > *>(ptr)->data; }
    pointer operator->() const { return &(static_cast<detail::Node< T > *>(ptr)->data); }

    LIter& operator++()
    {
      ptr = ptr->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter temp = *this;
      ptr = ptr->next;
      return temp;
    }

    bool operator==(const LIter& other) const { return ptr == other.ptr; }
    bool operator!=(const LIter& other) const { return ptr != other.ptr; }
  };

  template < class T > class LCIter
  {
    friend class List< T >;

  private:
    const detail::BaseNode* ptr;
    explicit LCIter(const detail::BaseNode* p) : ptr(p) {}

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    LCIter() : ptr(nullptr) {}
    LCIter(const LIter< T >& other) : ptr(other.ptr) {}

    reference operator*() const { return static_cast<const detail::Node< T > *>(ptr)->data; }
    pointer operator->() const { return &(static_cast<const detail::Node< T > *>(ptr)->data); }

    LCIter& operator++()
    {
      ptr = ptr->next;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter temp = *this;
      ptr = ptr->next;
      return temp;
    }

    bool operator==(const LCIter& other) const { return ptr == other.ptr; }
    bool operator!=(const LCIter& other) const { return ptr != other.ptr; }
  };

  template < class T > class List
  {
  private:
    detail::BaseNode* fakeNode;

  public:
    List() : fakeNode(new detail::BaseNode())
    {
      fakeNode->next = fakeNode;
    }

    List(const List& other) : fakeNode(new detail::BaseNode())
    {
      fakeNode->next = fakeNode;
      LIter< T > tail = end();
      for (auto it = other.cbegin(); it != other.cend(); ++it) {
        if (empty()) {
          pushFront(*it);
          tail = begin();
        }
        else {
          insertAfter(tail, *it);
          ++tail;
        }
      }
    }

    List(List&& other) noexcept : fakeNode(other.fakeNode)
    {
      other.fakeNode = new detail::BaseNode();
      other.fakeNode->next = other.fakeNode;
    }

    List& operator=(List other)
    {
      std::swap(fakeNode, other.fakeNode);
      return *this;
    }

    ~List()
    {
      clear();
      delete fakeNode;
    }

    void pushFront(const T& val)
    {
      detail::Node< T >* newNode = new detail::Node< T >(val);
      newNode->next = fakeNode->next;
      fakeNode->next = newNode;
    }

    void popFront() noexcept
    {
      if (!empty()) {
        detail::BaseNode* temp = fakeNode->next;
        fakeNode->next = temp->next;
        delete static_cast<detail::Node< T >*>(temp);
      }
    }

    void insertAfter(LIter< T > pos, const T& val)
    {
      if (pos.ptr) {
        detail::Node< T >* newNode = new detail::Node< T >(val);
        newNode->next = pos.ptr->next;
        pos.ptr->next = newNode;
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

    T& front()
    {
      return static_cast<detail::Node< T > *>(fakeNode->next)->data;
    }

    const T& front() const
    {
      return static_cast<const detail::Node< T > *>(fakeNode->next)->data;
    }

    LIter< T > begin() { return LIter< T >(fakeNode->next); }
    LIter< T > end() { return LIter< T >(fakeNode); }

    LCIter< T > cbegin() const { return LCIter< T >(fakeNode->next); }
    LCIter< T > cend() const { return LCIter< T >(fakeNode); }

    LCIter< T > begin() const { return cbegin(); }
    LCIter< T > end() const { return cend(); }
  };

}

#endif
