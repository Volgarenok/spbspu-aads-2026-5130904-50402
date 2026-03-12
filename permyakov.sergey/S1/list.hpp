#ifdef LIST_HPP
#define LIST_HPP

namespace permyakov
{
  template < class T > class List
  {
    T val;
    List <T> * next;
  };
}

#endif