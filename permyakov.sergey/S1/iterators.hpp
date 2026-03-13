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
      T &operator*();
      LIter &operator++();
      bool &operator==(LIter < T > &ScndIter);
      bool &operator!=(LIter < T > &ScndIter);
  };

  template < class T > class LCIter
  {
    friend class List < T >;
    const Node < T > * curr;
    public:
      const T &operator*();
      LCIter &operator++();
      bool &operator==(LCIter < T > &ScndIter);
      bool &operator!=(LCIter < T > &ScndIter);
  };
}

#endif