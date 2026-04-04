#ifndef LIST_HPP
#define LIST_HPP

#include <utility>

namespace samarin {

  namespace detail {
    template< class T >
    struct node_t {
      T value;
      node_t< T > * next;
    };
  }

  template< class T > class List;
  template< class T > class LCIter;

  template< class T >
  class LIter {
  public:
    T& operator*() const
    {
      return node_->value;
    }

    T* operator->() const
    {
      return &node_->value;
    }

    LIter< T >& operator++()
    {
      node_ = node_->next;
      return *this;
    }

    LIter< T > operator++(int)
    {
      LIter< T > tmp = *this;
      node_ = node_->next;
      return tmp;
    }

    bool operator==(const LIter< T >& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LIter< T >& other) const
    {
      return node_ != other.node_;
    }

  private:
    friend class List< T >;
    friend class LCIter< T >;
    detail::node_t< T > * node_;

    LIter():
      node_(nullptr)
    {}

    explicit LIter(detail::node_t< T > * n):
      node_(n)
    {}
  };

  template< class T >
  class LCIter {
  public:
    LCIter():
      node_(nullptr)
    {}

    LCIter(const LIter< T >& it):
      node_(it.node_)
    {}

    const T& operator*() const
    {
      return node_->value;
    }

    const T* operator->() const
    {
      return &node_->value;
    }

    LCIter< T >& operator++()
    {
      node_ = node_->next;
      return *this;
    }

    LCIter< T > operator++(int)
    {
      LCIter< T > tmp = *this;
      node_ = node_->next;
      return tmp;
    }

    bool operator==(const LCIter< T >& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LCIter< T >& other) const
    {
      return node_ != other.node_;
    }

  private:
    friend class List< T >;
    const detail::node_t< T > * node_;

    explicit LCIter(const detail::node_t< T > * n):
      node_(n)
    {}
  };

  template< class T >
  class List {
  public:
    List():
      head_(new detail::node_t< T >)
    {
      head_->next = nullptr;
    }

    List(const List< T >& other):
      head_(new detail::node_t< T >)
    {
      head_->next = nullptr;
      try {
        LIter< T > tail = before_begin();
        for (LCIter< T > it = other.cbegin(); it != other.cend(); ++it) {
          tail = insert_after(tail, *it);
        }
      } catch (...) {
        clear();
        delete head_;
        throw;
      }
    }

    List(List< T >&& other) noexcept:
      head_(other.head_)
    {
      other.head_ = nullptr;
    }

    ~List()
    {
      if (head_ != nullptr) {
        clear();
        delete head_;
      }
    }

    List< T >& operator=(const List< T >& other)
    {
      if (this != &other) {
        List< T > tmp(other);
        std::swap(head_, tmp.head_);
      }
      return *this;
    }

    List< T >& operator=(List< T >&& other) noexcept
    {
      if (this != &other) {
        clear();
        delete head_;
        head_ = other.head_;
        other.head_ = nullptr;
      }
      return *this;
    }

    void push_front(const T& v)
    {
      detail::node_t< T > * node = new detail::node_t< T >;
      node->value = v;
      node->next = head_->next;
      head_->next = node;
    }

    LIter< T > insert_after(LIter< T > pos, const T& v)
    {
      detail::node_t< T > * node = new detail::node_t< T >;
      node->value = v;
      node->next = pos.node_->next;
      pos.node_->next = node;
      return LIter< T >(node);
    }

    LIter< T > before_begin()
    {
      return LIter< T >(head_);
    }

    T& front()
    {
      return head_->next->value;
    }

    const T& front() const
    {
      return head_->next->value;
    }

    void pop_front()
    {
      detail::node_t< T > * temp = head_->next;
      head_->next = temp->next;
      delete temp;
    }

    bool empty() const
    {
      return head_->next == nullptr;
    }

    void clear();

    LIter< T > begin()
    {
      return LIter< T >(head_->next);
    }

    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }

    LCIter< T > begin() const
    {
      return LCIter< T >(head_->next);
    }

    LCIter< T > end() const
    {
      return LCIter< T >(nullptr);
    }

    LCIter< T > cbegin() const
    {
      return LCIter< T >(head_->next);
    }

    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }

    LCIter< T > cbefore_begin() const
    {
      return LCIter< T >(head_);
    }

  private:
    detail::node_t< T > * head_;
  };

  template< class T >
  void List< T >::clear()
  {
    detail::node_t< T > * temp = head_->next;
    while (temp != nullptr) {
      detail::node_t< T > * next = temp->next;
      delete temp;
      temp = next;
    }
    head_->next = nullptr;
  }

}

#endif