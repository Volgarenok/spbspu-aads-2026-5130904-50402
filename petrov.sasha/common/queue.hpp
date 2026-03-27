#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "list.hpp"
#include <stdexcept>

namespace petrov {
  template< class T >
  class Queue {
    public:
      void push(const T& rhs) {
        list_.pushBack(rhs);
      }

      void pop() {
        if (list_.empty()) {
          throw std::logic_error("Queue empty");
        }
        list_.popFront();
      }

      const T& front() const {
        if (list_.empty()) {
          throw std::logic_error("Queue empty");
        }
        return list_.front();
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