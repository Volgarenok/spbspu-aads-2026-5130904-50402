#ifndef STACK_HPP
#define STACK_HPP

#include "list.hpp"
#include <stdexcept>

namespace petrov {
  template< class T >
  class Stack {
    public:
      void push(const T& rhs) {
        list_.pushBack(rhs);
      }

      void pop() {
        if (list_.empty()) {
          throw std::logic_error("Stack empty");
        }
        list_.popBack();
      }

      const T& top() const {
        if (list_.empty()) {
          throw std::logic_error("Stack empty");
        }
        return list_.back();
      }

      bool empty() const {
        return list_.empty();
      }

      std::size_t size() const {
        return list_.size();
      }
    private:
      List< T > list_;
  };
}

#endif
