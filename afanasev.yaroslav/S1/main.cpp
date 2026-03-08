#include <iostream>


namespace afanasev
{
  template< class T > class Node;
  template< class T > class List;
  template< class T > class LIter;
  template< class T > class LCIter;

// узел
  template< class T >
  class Node
  {
    T val_;
    Node< T > * next_;
  };

// Список я так понял именно с ним будет работать пользователь
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

    void insert(const T & val, LIter< T > pos);
    void clear() noexcept;
    size_t size() const noexcept;
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
  void List< T >::insert(const T & val, LIter< T > pos)
  {
    Node< T > * new_node = new Node< T >{val, pos.curr_->next_};
    pos.curr_->next_ = new_node;
    ++size_;
  }

  template < class T >
  void List< T >::clear() noexcept
  {
    Node< T > * h = fake_->next_;
    while (h)
    {
      Node< T > * tmp = h->next;
      delete h;
      h = tmp;
    }
    fake_->next = nullptr;
    size_ = 0;
  }

  template < class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }



// Обычный итератор
  template < class T >
  class LIter
  {
    friend class List< T >;
    Node< T > * curr_;

  public:
    LIter(Node< T > * p);
    T & operator*() const;
    LIter & operator++();
    LIter & operator++(int);
    bool operator==(const LIter< T > & other) const;
    bool operator!=(const LIter< T > & other) const;
  };

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



// Константный итератор
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
    curr_ = curr_->val_;
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


int main()
{
  return 0;
}
