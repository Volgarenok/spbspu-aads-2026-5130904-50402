#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "node.hpp"

namespace dirko
{
  template < class T >
  class LIter;
  template < class T >
  class LCIter;
  template < class T >
  class List
  {
  public:
    List();
    List(const List< T > &);
    List(List< T > &&) noexcept;
    List &operator=(const List< T > &);
    List &operator=(List< T > &&) noexcept;
    ~List();
    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
    T &head() noexcept;
    T &tail() noexcept;
    const T &chead() const noexcept;
    const T &ctail() const noexcept;

    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();

    void clear();
    size_t size() const noexcept;
    void swap(List< T > &) noexcept;
    LIter< T > insert(LIter< T > pos, const T &value);
    LIter< T > erase(LIter< T > pos);

  private:
    Node< T > *fake_;
    Node< T > *tail_;
    size_t size_;
  };

  template < class T >
  List< T >::List():
    fake_(static_cast< Node< T > * >(::operator new(sizeof(Node< T >)))),
    tail_(fake_),
    size_(0)
  {
    fake_->next = nullptr;
    fake_->prev = nullptr;
  }
  template < class T >
  List< T >::List(List< T > &&other) noexcept:
    fake_(other.fake_),
    tail_(other.tail_),
    size_(other.size_)
  {
    other.fake_ = nullptr;
    other.size_ = 0;
  }
  template < class T >
  List< T >::List(const List< T > &other):
    fake_(static_cast< Node< T > * >(::operator new(sizeof(Node< T >)))),
    tail_(fake_),
    size_(0)
  {
    fake_->next = nullptr;
    fake_->prev = nullptr;
    Node< T > *head = other.fake_->next;
    while (head) {
      push_back(head->val);
      head = head->next;
    }
  }
  template < class T >
  List< T > &List< T >::operator=(List< T > &&other) noexcept
  {
    clear();
    ::operator delete(fake_);
    fake_ = other.fake_;
    size_ = other.size_;
    tail_ = other.tail_;
    other.fake_ = nullptr;
    other.size_ = 0;
    return *this;
  }
  template < class T >
  List< T > &List< T >::operator=(const List< T > &other)
  {
    List< T > tmp = List< T >(other);
    swap(tmp);
    return *this;
  }
  template < class T >
  List< T >::~List()
  {
    clear();
    ::operator delete(fake_);
  }
  template < class T >
  void List< T >::clear()
  {
    if (fake_) {

      Node< T > *head = fake_->next;
      while (head != nullptr) {
        Node< T > *tmp = head->next;
        delete head;
        head = tmp;
      }
      fake_->next = nullptr;
    }
    size_ = 0;
    tail_ = fake_;
  }
  template < class T >
  void List< T >::swap(List< T > &rhs) noexcept
  {
    std::swap(fake_, rhs.fake_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }
  template < class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }
  template < class T >
  void List< T >::push_front(const T &val)
  {
    Node< T > *n = new Node< T >{val, fake_->next, nullptr};
    if (!size_) {
      fake_->next = n;
      tail_ = n;
    } else {
      fake_->next->prev = n;
      fake_->next = n;
    }
    ++size_;
  }
  template < class T >
  void List< T >::push_back(const T &val)
  {
    Node< T > *n = new Node< T >{val, nullptr, tail_};
    if (!size_) {
      fake_->next = n;
    } else {
      tail_->next = n;
    }
    tail_ = n;
    ++size_;
  }
  template < class T >
  void List< T >::pop_front()
  {
    if (size_ == 0) {
      throw std::logic_error("Empty list");
    }
    Node< T > *next = fake_->next->next;
    delete fake_->next;
    fake_->next = next;
    if (!next) {
      tail_ = fake_;
    }
    --size_;
  }
  template < class T >
  void List< T >::pop_back()
  {
    if (size_ == 0) {
      throw std::logic_error("Empty list");
    }
    Node< T > *prev = tail_->prev;
    delete tail_;
    if (!prev) {
      tail_ = fake_;
    } else {
      tail_ = prev;
      tail_->next = nullptr;
    }
    --size_;
  }

  template < class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >{fake_->next};
  }

  template < class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >{tail_->next};
  }

  template < class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >{fake_->next};
  }

  template < class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >{tail_->next};
  }
  template < class T >
  T &List< T >::head() noexcept
  {
    return fake_->next->val;
  }
  template < class T >
  T &List< T >::tail() noexcept
  {
    return tail_->val;
  }
  template < class T >
  const T &List< T >::chead() const noexcept
  {
    return fake_->next->val;
  }
  template < class T >
  const T &List< T >::ctail() const noexcept
  {
    return tail_->val;
  }
  template < class T >
  class LIter
  {
  public:
    LIter(Node< T > *);
    T &operator*() const;
    LIter &operator++();
    LIter &operator--();
    LIter operator++(int);
    LIter operator--(int);
    bool operator==(const LIter< T > &) const;
    bool operator!=(const LIter< T > &) const;

  private:
    Node< T > *curr_;
    friend class List< T >;
  };
  template < class T >
  class LCIter
  {
  public:
    LCIter(Node< T > *);
    const T &operator*() const;
    LCIter &operator++();
    LCIter &operator--();
    LCIter operator++(int);
    LCIter operator--(int);
    bool operator==(const LCIter< T > &) const;
    bool operator!=(const LCIter< T > &) const;

  private:
    const Node< T > *curr_;
    friend class List< T >;
  };
  template < class T >
  LIter< T >::LIter(Node< T > *node):
    curr_(node)
  {}
  template < class T >
  T &LIter< T >::operator*() const
  {
    return curr_->val;
  }
  template < class T >
  bool LIter< T >::operator==(const LIter< T > &rhs) const
  {
    return curr_ == rhs.curr_;
  }
  template < class T >
  bool LIter< T >::operator!=(const LIter< T > &rhs) const
  {
    return !(*this == rhs);
  }
  template < class T >
  LIter< T > &LIter< T >::operator++()
  {
    curr_ = curr_->next;
    return *this;
  }
  template < class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > prev = curr_;
    curr_ = curr_->next;
    return prev;
  }
  template < class T >
  LIter< T > &LIter< T >::operator--()
  {
    curr_ = curr_->prev;
    return *this;
  }
  template < class T >
  LIter< T > LIter< T >::operator--(int)
  {
    LIter< T > next = curr_;
    curr_ = curr_->prev;
    return next;
  }
  template < class T >
  LCIter< T >::LCIter(Node< T > *node):
    curr_(node)
  {}
  template < class T >
  const T &LCIter< T >::operator*() const
  {
    return curr_->val;
  }
  template < class T >
  bool LCIter< T >::operator==(const LCIter< T > &rhs) const
  {
    return curr_ == rhs.curr_;
  }
  template < class T >
  bool LCIter< T >::operator!=(const LCIter< T > &rhs) const
  {
    return !(*this == rhs);
  }
  template < class T >
  LCIter< T > &LCIter< T >::operator++()
  {
    curr_ = curr_->next;
    return *this;
  }
  template < class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > prev = curr_;
    curr_ = curr_->next;
    return prev;
  }
  template < class T >
  LCIter< T > &LCIter< T >::operator--()
  {
    curr_ = curr_->prev;
    return *this;
  }
  template < class T >
  LCIter< T > LCIter< T >::operator--(int)
  {
    LCIter< T > next = curr_;
    curr_ = curr_->prev;
    return next;
  }
  template < class T >
  LIter< T > List< T >::insert(LIter< T > pos, const T &value)
  {
    Node< T > *posNode = pos.curr_;
    Node< T > *newNode = new Node< T >{value, posNode, posNode->prev};
    if (posNode->prev) {
      posNode->prev->next = newNode;
      posNode->prev = newNode;
    }
    size_++;
    return LIter< T >{newNode};
  }
  template < class T >
  LIter< T > List< T >::erase(LIter< T > pos)
  {
    if (size_ == 0) {
      throw std::logic_error("Empty list");
    }
    Node< T > *next = pos.curr_->next;
    Node< T > *prev = pos.curr_->prev;
    delete pos.curr_;
    if (next) {
      next->prev = prev;
    }
    if (prev) {
      prev->next = next;
    }
    --size_;
    if (!size_) {
      tail_ = fake_;
    }
    return {next};
  }
}

#endif
