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
    List() noexcept;
    ~List() noexcept;
    void clear() noexcept;
    void pushBack(const T& value);

  private:
    Node< T > m_fake;
    Node< T >* m_tail;
  };
}

#endif
