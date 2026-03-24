#ifndef STACK_HPP
#define STACK_HPP

namespace afanasev
{
  template< typename T >
	class Stack
	{
	 public:
	  void push(const T &rhs);
    T &get() noexcept;
    void pop() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

		void push(T rhs);
		T drop();
  };
}

#endif
