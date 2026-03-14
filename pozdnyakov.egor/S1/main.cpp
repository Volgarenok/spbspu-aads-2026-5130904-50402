#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <limits>

namespace pozdnyakov
{

  namespace detail
  {
    template < class T > struct Node
    {
      T data;
      Node *next;
      Node(const T &val, Node *n = nullptr):
        data(val),
        next(n)
      {}
    };
  }

  template < class T > class List;
  template < class T > class LCIter;

  template < class T > class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;
  private:
    detail::Node< T > *ptr;
    explicit LIter(detail::Node< T > *p):
      ptr(p)
    {}

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    LIter():
      ptr(nullptr)
    {}

    reference operator*() const
    {
      return ptr->data;
    }
    pointer operator->() const
    {
      return &(ptr->data);
    }

    LIter &operator++()
    {
      if (ptr)
        ptr = ptr->next;
      return *this;
    }

    LIter operator++(int)
    {
      LIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter &other) const
    {
      return ptr == other.ptr;
    }
    bool operator!=(const LIter &other) const
    {
      return ptr != other.ptr;
    }
  };

  template < class T > class LCIter
  {
    friend class List< T >;

  private:
    const detail::Node< T > *ptr;
    explicit LCIter(const detail::Node< T > *p):
      ptr(p)
    {}

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    LCIter():
      ptr(nullptr)
    {}
    LCIter(const LIter< T > &other):
      ptr(other.ptr)
    {}

    reference operator*() const
    {
      return ptr->data;
    }
    pointer operator->() const
    {
      return &(ptr->data);
    }

    LCIter &operator++()
    {
      if (ptr)
        ptr = ptr->next;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LCIter &other) const
    {
      return ptr == other.ptr;
    }
    bool operator!=(const LCIter &other) const
    {
      return ptr != other.ptr;
    }
  };

  template < class T > class List
  {
  private:
    detail::Node< T > *head;

  public:
    List() noexcept:
      head(nullptr)
    {}

    ~List() noexcept
    {
      clear();
    }

    List(const List &other):
      head(nullptr)
    {
      if (other.empty())
        return;

      try {
        auto it = other.cbegin();
        push_front(*it);
        LIter< T > tail = begin();
        ++it;

        while (it != other.cend()) {
          insert_after(tail, *it);
          ++tail;
          ++it;
        }
      } catch (...) {
        clear();
        throw;
      }
    }

    List(List &&other) noexcept:
      head(other.head)
    {
      other.head = nullptr;
    }

    List &operator=(const List &other)
    {
      if (this != &other) {
        List tmp(other);
        std::swap(head, tmp.head);
      }
      return *this;
    }

    List &operator=(List &&other) noexcept
    {
      if (this != &other) {
        clear();
        head = other.head;
        other.head = nullptr;
      }
      return *this;
    }

    void push_front(const T &val)
    {
      head = new detail::Node< T >(val, head);
    }

    void pop_front() noexcept
    {
      if (head) {
        detail::Node< T > *temp = head;
        head = head->next;
        delete temp;
      }
    }

    void insert_after(LIter< T > pos, const T &val)
    {
      if (pos.ptr) {
        pos.ptr->next = new detail::Node< T >(val, pos.ptr->next);
      }
    }

    void erase_after(LIter< T > pos) noexcept
    {
      if (pos.ptr && pos.ptr->next) {
        detail::Node< T > *temp = pos.ptr->next;
        pos.ptr->next = temp->next;
        delete temp;
      }
    }

    void clear() noexcept
    {
      while (head) {
        pop_front();
      }
    }

    bool empty() const noexcept
    {
      return head == nullptr;
    }

    T &front()
    {
      return head->data;
    }
    const T &front() const
    {
      return head->data;
    }

    LIter< T > begin()
    {
      return LIter< T >(head);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    LCIter< T > cbegin() const
    {
      return LCIter< T >(head);
    }
    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }
  };

  using ValueType = unsigned long long;
  using NamedSequence = std::pair< std::string, List< ValueType > >;

  struct ProcessResult
  {
    List< List< ValueType > > rows;
    List< ValueType > sums;
  };

  ProcessResult build_interleaved_rows(const List< NamedSequence > &sequences)
  {
    ProcessResult result;

    List< LCIter< ValueType > > iters;
    List< LCIter< ValueType > > ends;

    LIter< LCIter< ValueType > > iters_tail = iters.end();
    LIter< LCIter< ValueType > > ends_tail = ends.end();

    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
      if (iters.empty()) {
        iters.push_front((*it).second.cbegin());
        ends.push_front((*it).second.cend());
        iters_tail = iters.begin();
        ends_tail = ends.begin();
      } else {
        iters.insert_after(iters_tail, (*it).second.cbegin());
        ends.insert_after(ends_tail, (*it).second.cend());
        ++iters_tail;
        ++ends_tail;
      }
    }

    LIter< List< ValueType > > rows_tail = result.rows.end();
    LIter< ValueType > sums_tail = result.sums.end();

    while (true) {
      bool elements_left = false;
      ValueType current_row_sum = 0;

      List< ValueType > current_row;
      LIter< ValueType > current_row_tail = current_row.end();

      auto it = iters.begin();
      auto end_it = ends.begin();

      for (; it != iters.end() && end_it != ends.end(); ++it, ++end_it) {
        if (*it != *end_it) {
          elements_left = true;
          ValueType val = *(*it);

          if (current_row.empty()) {
            current_row.push_front(val);
            current_row_tail = current_row.begin();
          } else {
            current_row.insert_after(current_row_tail, val);
            ++current_row_tail;
          }

          if (current_row_sum > std::numeric_limits< ValueType >::max() - val) {
            throw std::overflow_error("Произошло переполнение при подсчете суммы.");
          }
          current_row_sum += val;
          ++(*it);
        }
      }

      if (!elements_left)
        break;

      if (result.rows.empty()) {
        result.rows.push_front(current_row);
        rows_tail = result.rows.begin();
      } else {
        result.rows.insert_after(rows_tail, current_row);
        ++rows_tail;
      }

      if (result.sums.empty()) {
        result.sums.push_front(current_row_sum);
        sums_tail = result.sums.begin();
      } else {
        result.sums.insert_after(sums_tail, current_row_sum);
        ++sums_tail;
      }
    }

    return result;
  }

}
