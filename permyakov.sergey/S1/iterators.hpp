#ifdef ITERATORS_HPP
#define ITERATORS_HPP

namespace permyakov
{
  template < class T > class List;

  template < class T > class LIter
  {
    friend class List;
    List < T > * curr;
    public:
      T &operator*();
      LIter &operator++();
      LIter &operator==(LIter < T > &ScndIter);
      LIter &operator!=(LIter < T > &ScndIter);
  };

  template < class T > class LCIter
  {
    friend class List;
    const List < T > * curr;
    public:
      const T LCIter &operator*();
      LCIter &operator++();
      LCIter &operator==(LCIter < T > &ScndIter);
      LCIter &operator!=(LCIter < T > &ScndIter);
  };
}

#endif