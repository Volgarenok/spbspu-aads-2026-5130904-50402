#include "../common/list.hpp"

namespace dirko
{
  template < class T >
  class Queue
  {
  public:
    void push(const T &rhs);
    T &pop();

  private:
    List< T > data_;
  };

  template < class T >
  class Stack
  {
  public:
    void push(const T &rhs);
    T &pop();

  private:
    List< T > data_;
  };
}

int main()
{}
