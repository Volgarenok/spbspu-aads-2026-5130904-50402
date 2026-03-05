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

  template< class T > class List;

  template< class T >
  class LIter
  {
    friend class List< T >;

  public:
    LIter< T > next() const noexcept;

  private:
    LIter(Node< T >* node) noexcept;

  private:
    Node< T >* m_cur;
  };

  template< class T >
  class List
  {
  public:
    List() noexcept;
    ~List() noexcept;
    void clear() noexcept;
    LIter< T > begin() const noexcept;
    LIter< T > end() const noexcept;
    void pushBack(const T& value);

  private:
    Node< T > m_fake;
    Node< T >* m_tail;
  };
}

#endif
