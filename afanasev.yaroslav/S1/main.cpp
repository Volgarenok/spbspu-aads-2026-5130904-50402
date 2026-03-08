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

    void clear();
    size_t size() const;
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
