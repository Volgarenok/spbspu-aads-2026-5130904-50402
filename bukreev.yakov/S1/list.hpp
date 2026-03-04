#ifndef LIST_HPP
#define LIST_HPP

namespace bukreev
{
  template< class T >
  struct Node
  {
    T val;
    Node< T >* next;
  };

  template< class T >
  class List
  {
  public:
    List();

  private:
    Node< T > m_fake;
  };
}

#endif
