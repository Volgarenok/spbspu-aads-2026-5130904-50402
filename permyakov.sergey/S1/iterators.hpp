#ifndef ITERATORS_HPP
#define ITERATORS_HPP
#include "node.hpp"

namespace permyakov
{
  template < class T > class List;

  template < class T > class LIter
  {
    friend class List < T >;
    Node < T > * curr;
    public:
      T & operator*();
      LIter & operator++();
      bool & operator==(LIter < T > & scndIter);
      bool & operator!=(LIter < T > & scndIter);
  };

  template < class T > class LCIter
  {
    friend class List < T >;
    const Node < T > * curr;
    public:
      const T & operator*();
      LCIter & operator++();
      bool & operator==(LCIter < T > & scndIter);
      bool & operator!=(LCIter < T > & scndIter);
  };

  template < class T > T & LIter < T >::operator*()
  {
    return curr -> val;
  }

  template < class T > const T & LCIter < T >::operator*()
  {
    return curr -> val;
  }
  
  template < class T > LIter < T > & LIter < T >::operator++()
  {
    curr = curr -> next;
    return *this;
  }

  template < class T > LCIter < T > & LCIter < T >::operator++()
  {
    curr = curr -> next;
    return *this;
  }

  template < class T > bool & LIter < T >::operator==(LIter < T > & scndIter)
  {
    return curr == scndIter.curr;
  }

  template < class T > bool & LIter < T >::operator!=(LIter < T > & scndIter)
  {
    return curr != scndIter.curr;
  }

  template < class T > bool & LCIter < T >::operator==(LCIter < T > & scndIter)
  {
    return curr == scndIter.curr;
  }

  template < class T > bool & LCIter < T >::operator!=(LCIter < T > & scndIter)
  {
    return curr != scndIter.curr;
  }
}

#endif