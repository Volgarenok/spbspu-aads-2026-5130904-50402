#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "node.hpp"

namespace dirko
{
  template < class T >
  class Iter;
  template < class T >
  class CIter;
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

    Iter< T > begin();
    Iter< T > end();
    CIter< T > cbegin() const;
    CIter< T > cend() const;

    T &head() noexcept;
    T &tail() noexcept;
    const T &chead() const noexcept;
    const T &ctail() const noexcept;

    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();
    void erase(Iter< T > pos);

    void clear();
    size_t size() const noexcept;
    void swap(List< T > &) noexcept;

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
  void List< T >::erase(Iter< T > pos)
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
  }

  template < class T >
  Iter< T > List< T >::begin()
  {
    return Iter< T >{fake_->next};
  }

  template < class T >
  Iter< T > List< T >::end()
  {
    return Iter< T >{tail_->next};
  }

  template < class T >
  CIter< T > List< T >::cbegin() const
  {
    return CIter< T >{fake_->next};
  }

  template < class T >
  CIter< T > List< T >::cend() const
  {
    return CIter< T >{tail_->next};
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
  class Iter
  {
  public:
    Iter(Node< T > *);
    T &operator*() const;
    Iter &operator++();
    Iter &operator--();
    Iter operator++(int);
    Iter operator--(int);
    bool operator==(const Iter< T > &) const;
    bool operator!=(const Iter< T > &) const;

  private:
    Node< T > *curr_;
    friend class List< T >;
  };
  template < class T >
  class CIter
  {
  public:
    CIter(Node< T > *);
    const T &operator*() const;
    CIter &operator++();
    CIter &operator--();
    CIter operator++(int);
    CIter operator--(int);
    bool operator==(const CIter< T > &) const;
    bool operator!=(const CIter< T > &) const;

  private:
    const Node< T > *curr_;
    friend class List< T >;
  };
  template < class T >
  Iter< T >::Iter(Node< T > *node):
    curr_(node)
  {}
  template < class T >
  T &Iter< T >::operator*() const
  {
    return curr_->val;
  }
  template < class T >
  bool Iter< T >::operator==(const Iter< T > &rhs) const
  {
    return curr_ == rhs.curr_;
  }
  template < class T >
  bool Iter< T >::operator!=(const Iter< T > &rhs) const
  {
    return !(*this == rhs);
  }
  template < class T >
  Iter< T > &Iter< T >::operator++()
  {
    curr_ = curr_->next;
    return *this;
  }
  template < class T >
  Iter< T > Iter< T >::operator++(int)
  {
    Iter< T > prev = curr_;
    curr_ = curr_->next;
    return prev;
  }
  template < class T >
  Iter< T > &Iter< T >::operator--()
  {
    curr_ = curr_->prev;
    return *this;
  }
  template < class T >
  Iter< T > Iter< T >::operator--(int)
  {
    Iter< T > next = curr_;
    curr_ = curr_->prev;
    return next;
  }
  template < class T >
  CIter< T >::CIter(Node< T > *node):
    curr_(node)
  {}
  template < class T >
  const T &CIter< T >::operator*() const
  {
    return curr_->val;
  }
  template < class T >
  bool CIter< T >::operator==(const CIter< T > &rhs) const
  {
    return curr_ == rhs.curr_;
  }
  template < class T >
  bool CIter< T >::operator!=(const CIter< T > &rhs) const
  {
    return !(*this == rhs);
  }
  template < class T >
  CIter< T > &CIter< T >::operator++()
  {
    curr_ = curr_->next;
    return *this;
  }
  template < class T >
  CIter< T > CIter< T >::operator++(int)
  {
    CIter< T > prev = curr_;
    curr_ = curr_->next;
    return prev;
  }
  template < class T >
  CIter< T > &CIter< T >::operator--()
  {
    curr_ = curr_->prev;
    return *this;
  }
  template < class T >
  CIter< T > CIter< T >::operator--(int)
  {
    CIter< T > next = curr_;
    curr_ = curr_->prev;
    return next;
  }
}
#endif
