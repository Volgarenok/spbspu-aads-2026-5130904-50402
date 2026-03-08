#ifndef LIST_BASE_HPP
#define LIST_BASE_HPP

#include "vanila_list.hpp"
#include "node.hpp"
#include "LCIter.hpp"
#include "LIter.hpp"
#include <cstddef>
#include <stdexcept>

namespace chernikov {

  template < typename T > class List
  {
  private:
    Node< T > *head;
    size_t count;

  public:
    List():
      head(nullptr),
      count(0)
    {
    }
    List(const List &other):
      head(nullptr),
      count(0)
    {
      copy_from(other);
    }
    List(List &&other) noexcept:
      head(other.head),
      count(other.count)
    {
      other.head = nullptr;
      other.count = 0;
    }
    ~List()
    {
      clear();
    }

    bool empty() const
    {
      return head == nullptr;
    }
    size_t size() const
    {
      return count;
    }

    LIter< T > begin()
    {
      return LIter< T >(head);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    LCIter< T > begin() const
    {
      return LCIter< T >(head);
    }
    LCIter< T > end() const
    {
      return LCIter< T >(nullptr);
    }

    LCIter< T > cbegin() const
    {
      return LCIter< T >(head);
    }
    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }

    T &front()
    {
      if (empty())
        throw std::logic_error("List is empty");
      return head->data;
    }

    const T &front() const
    {
      if (empty())
        throw std::logic_error("List is empty");
      return head->data;
    }
    void swap(List &other) noexcept
    {
      std::swap(head, other.head);
      std::swap(count, other.count);
    }
    void List< T >::add(const T &val);
    void clear();
    List< T > &operator=(const List &other);
    List< T > &List< T >::operator=(List &&other) noexcept;

  private:
    void copy_from(const List &other);
    void clear();
    void first_delete()
  };
}

#endif
