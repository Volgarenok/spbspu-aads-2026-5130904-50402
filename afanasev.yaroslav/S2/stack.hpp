#ifndef STACK_HPP
#define STACK_HPP

namespace afanasev
{
  template< typename T >
	class Stack
	{
	 public:
	  void push(const T & rhs);
    T & get() noexcept;
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

	 private:
    List< T > data_;
  };
}

template < class T >
void afanasev::Stack< T >::push(const T & rhs)
{

}

template < class T >
T & afanasev::Stack< T >::get()
{

}

template < class T >
void afanasev::Stack< T >::pop()
{

}

template < class T >
bool afanasev::Stack< T >::empty() const noexcept
{

}

template < class T >
size_t afanasev::Stack< T >::size() const noexcept
{

}

#endif
