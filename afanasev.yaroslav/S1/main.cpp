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
  template < class T > class LIter
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

// Константный итератор
  template < class T >
  class LCIter
  {
    friend class List< T >;
    const List< T > * curr;

  public:
    LCIter(const List< T > * p);
    const T & operator*() const;
    LCIter & operator++();
    LCIter & operator++(int);
    bool operator==(const LCIter< T > & other) const;
    bool operator!=(const LCIter< T > & other) const;
  };
}

int main()
{
  return 0;
}
