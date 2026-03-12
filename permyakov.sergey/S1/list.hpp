#ifdef LIST_HPP
#define List_HPP

namespace permyakov
{
  template < class T > class List
  {
    T val;
    List <T> * next;
    public:
      List();
      List(const List < T > &);
      List(List < T > &&);
      List &operator=(const List < T > &);
      List &operator=(List < T > &&);
      ~List();
  }  
}

#endif