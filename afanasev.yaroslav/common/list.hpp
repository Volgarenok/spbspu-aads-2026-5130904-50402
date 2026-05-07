#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <utility>
#include <string>
#include <limits>


namespace afanasev
{
  template< class T > class Node;
  template< class T > class List;
  template< class T > class LIter;
  template< class T > class LCIter;

  template< class T >
  class Node
  {
  public:
    T val_;
    Node< T > * next_;
  };

  template< class T >
  class List
  {
    Node< T > * fake_;
    size_t size_;

  public:
    List();
    ~List();

    LIter< T > begin() noexcept;
    LCIter< T > begin() const noexcept;

    T getValue(LIter< T > pos) noexcept;
    T getValue(LCIter< T > pos) const noexcept;
    void pushFront(const T & val);
    T popFront();
    void insert(const T & val, LIter< T > pos);
    void deleteNext(LIter< T > pos) noexcept;
    void clear() noexcept;
    size_t size() const noexcept;

    List(const List & other);
    List(List && other) noexcept;
    List & operator=(const List & other);
    List & operator=(List && other) noexcept;
    void swap(List & other) noexcept;
  };

  template < class T >
  List< T >::List():
    fake_(new Node< T >{T(), nullptr}),
    size_(0)
  {}

  template < class T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template < class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >{fake_->next_};
  }

  template < class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >{fake_->next_};
  }

  template < class T >
  T List< T >::getValue(LIter< T > pos) noexcept
  {
    return pos.curr_->val_;
  }

  template < class T >
  T List< T >::getValue(LCIter< T > pos) const noexcept
  {
    return pos.curr_->val_;
  }

  template < class T >
  void List< T >::pushFront(const T & val)
  {
    Node< T > * new_node = new Node< T >{val, fake_->next_};
    fake_->next_ = new_node;
    ++size_;
  }

  template < class T >
  T List< T >::popFront()
  {
    if (!size_)
    {
      return nullptr;
    }
    Node< T > * first = fake_->next_;
    T value = first->val_;
    fake_->next_ = first->next_;
    delete first;
    --size_;
    return value;
  }

  template < class T >
  void List< T >::insert(const T & val, LIter< T > pos)
  {
    Node< T > * new_node = new Node< T >{val, pos.curr_->next_};
    pos.curr_->next_ = new_node;
    ++size_;
  }

  template < class T >
  void List< T >::deleteNext(LIter< T > pos) noexcept
  {
    Node< T > * tmp = pos.curr_->next_;
    if (!tmp)
    {
      return;
    }

    pos.curr_->next_ = tmp->next_;
    delete tmp;
    size_--;
  }

  template < class T >
  void List< T >::clear() noexcept
  {
    Node< T > * h = fake_->next_;
    while (h)
    {
      Node< T > * tmp = h->next_;
      delete h;
      h = tmp;
    }
    fake_->next_ = nullptr;
    size_ = 0;
  }

  template < class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }


  template < class T >
  List< T >::List(const List< T > & other) :
    fake_(new Node< T >{T(), nullptr}),
    size_(0)
  {
    Node< T > * this_curr = fake_;
    Node< T > * other_curr = other.fake_->next_;
    while (other_curr)
    {
      Node< T > * new_node = new Node< T >{other_curr->val_, nullptr};
      this_curr->next_ = new_node;
      this_curr = new_node;
      other_curr = other_curr->next_;
      ++size_;
    }
  }

  template < class T >
  List< T >::List(List< T > && other) noexcept :
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = new Node< T >{T(), nullptr};
    other.size_ = 0;
  }

  template < class T >
  List< T > & List< T >::operator=(const List< T > & other)
  {
    if (this != & other)
    {
      List tmp(other);
      swap(tmp);
    }
    return * this;
  }

  template < class T >
  List< T > & List< T >::operator=(List< T > && other) noexcept
  {
    if (this != & other)
    {
      clear();
      delete fake_;

      fake_ = other.fake_;
      size_ = other.size_;

      other.fake_ = new Node< T >{T(), nullptr};
      other.size_ = 0;
    }
    return * this;
  }

  template < class T >
  void List< T >::swap(List< T > & other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }


  template < class T >
  class LIter
  {
    friend class List< T >;
    Node< T > * curr_;

  public:
    LIter();
    LIter(Node< T > * p);
    T & operator*() const;
    LIter & operator++();
    LIter & operator++(int);
    bool operator==(const LIter< T > & other) const;
    bool operator!=(const LIter< T > & other) const;
  };

  template < class T >
  LIter< T >::LIter():
    curr_(nullptr)
  {}

  template < class T >
  LIter< T >::LIter(Node< T > * p):
    curr_(p)
  {}

  template < class T >
  T & LIter< T >::operator*() const
  {
    return curr_->val_;
  }

  template < class T >
  LIter< T > & LIter< T >::operator++()
  {
    curr_ = curr_->next_;
    return * this;
  }

  template < class T >
  LIter< T > & LIter< T >::operator++(int)
  {
    curr_ = curr_->next_;
    return * this;
  }

  template < class T >
  bool LIter< T >::operator==(const LIter< T > & other) const
  {
    return curr_ == other.curr_;
  }

  template < class T >
  bool LIter< T >::operator!=(const LIter< T > & other) const
  {
    return curr_ != other.curr_;
  }


  template < class T >
  class LCIter
  {
    friend class List< T >;
    const Node< T > * curr_;

  public:
    LCIter(const Node< T > * p);
    const T & operator*() const;
    LCIter & operator++();
    LCIter & operator++(int);
    bool operator==(const LCIter< T > & other) const;
    bool operator!=(const LCIter< T > & other) const;
  };

  template < class T >
  LCIter< T >::LCIter(const Node< T > * p):
    curr_(p)
  {}

  template < class T >
  const T & LCIter< T >::operator*() const
  {
    return curr_->val_;
  }

  template < class T >
  LCIter< T > & LCIter< T >::operator++()
  {
    curr_ = curr_->next_;
    return * this;
  }

  template < class T >
  LCIter< T > & LCIter< T >::operator++(int)
  {
    curr_ = curr_->next_;
    return * this;
  }

  template < class T >
  bool LCIter< T >::operator==(const LCIter< T > & other) const
  {
    return curr_ == other.curr_;
  }

  template < class T >
  bool LCIter< T >::operator!=(const LCIter< T > & other) const
  {
    return curr_ != other.curr_;
  }
}

#endif
