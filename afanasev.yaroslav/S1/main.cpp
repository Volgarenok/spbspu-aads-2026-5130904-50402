#include <iostream>


namespace afanasev
{
  template< class T > class List;
  template< class T > class LIter;
  template< class T > class LCIter;

// Список(узел)
  template< class T >
  class List
  {
    T val_;
    List< T > * next_;
  };


// Обычный итератор
  template < class T >
  class LIter
  {
    friend class List< T >;
    List< T > * curr_;

  public:
    LIter(List< T > * p);
    T & operator*() const;
    LIter & operator++();
    LIter & operator++(int);
    bool operator==(const LIter< T > & other) const;
    bool operator!=(const LIter< T > & other) const;
  };

  template < class T >
  LIter< T >::LIter(List< T > * p):
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
    const List< T > * curr_;

  public:
    LCIter(const List< T > * p);
    const T & operator*() const;
    LCIter & operator++();
    LCIter & operator++(int);
    bool operator==(const LCIter< T > & other) const;
    bool operator!=(const LCIter< T > & other) const;
  };

  template < class T >
  LCIter< T >::LCIter(const List< T > * p):
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
