#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <limits>
#include <vector>
#include <sstream>

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
            throw std::overflow_error("Overflow while sum counting");
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

  namespace tests
  {
    void assert_true(bool condition, const std::string &name)
    {
      if (!condition)
        throw std::runtime_error("Test failed: " + name);
      std::cout << "[OK] " << name << "\n";
    }

    List< ValueType > make_list(const std::vector< ValueType > &vals)
    {
      List< ValueType > lst;
      LIter< ValueType > tail = lst.end();
      for (auto v : vals) {
        if (lst.empty()) {
          lst.push_front(v);
          tail = lst.begin();
        } else {
          lst.insert_after(tail, v);
          ++tail;
        }
      }
      return lst;
    }

    void test_exception_safety()
    {
      List< int > list1;
      list1.push_front(10);
      list1.push_front(20);
      List< int > list2 = list1;
      assert_true(list2.front() == 20, "Copy constructor safe and valid");
    }

    void test_process_logic()
    {
      List< NamedSequence > seqs;
      seqs.push_front({"fourth", make_list({4, 4})});
      seqs.push_front({"third", make_list({})});
      seqs.push_front({"second", make_list({2, 2, 2, 2})});
      seqs.push_front({"first", make_list({1, 1, 1})});

      ProcessResult res = build_interleaved_rows(seqs);

      std::vector< ValueType > expected_sums = {7, 7, 3, 2};
      int i = 0;
      for (auto it = res.sums.begin(); it != res.sums.end(); ++it) {
        assert_true(*it == expected_sums[i++], "Sum matches expected");
      }
    }

    void run_all()
    {
      std::cout << "Tests started\n";
      test_exception_safety();
      test_process_logic();
      std::cout << "Tests completed\n";
    }
  }

}

int main(int argc, char *argv[])
{
  if (argc > 1 && std::string(argv[1]) == "--test") {
    try {
      pozdnyakov::tests::run_all();
      return 0;
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }

  try {
    pozdnyakov::List< pozdnyakov::NamedSequence > sequences;
    pozdnyakov::LIter< pozdnyakov::NamedSequence > seq_tail = sequences.end();

    std::string name;

    while (std::cin >> name) {
      if (sequences.empty()) {
        sequences.push_front({name, pozdnyakov::List< pozdnyakov::ValueType >()});
        seq_tail = sequences.begin();
      } else {
        sequences.insert_after(seq_tail, {name, pozdnyakov::List< pozdnyakov::ValueType >()});
        ++seq_tail;
      }

      pozdnyakov::LIter< pozdnyakov::ValueType > val_tail = seq_tail->second.end();
      pozdnyakov::ValueType val;

      while (std::cin >> val) {
        if (seq_tail->second.empty()) {
          seq_tail->second.push_front(val);
          val_tail = seq_tail->second.begin();
        } else {
          seq_tail->second.insert_after(val_tail, val);
          ++val_tail;
        }
      }

      if (std::cin.eof() || std::cin.bad()) {
        break;
      }

      std::cin.clear();
    }

    if (sequences.empty()) {
      std::cout << 0 << "\n";
      return 0;
    }

    bool first_name = true;
    for (auto it = sequences.begin(); it != sequences.end(); ++it) {
      if (!first_name)
        std::cout << " ";
      std::cout << (*it).first;
      first_name = false;
    }
    std::cout << "\n";

    pozdnyakov::ProcessResult result = pozdnyakov::build_interleaved_rows(sequences);

    for (auto row_it = result.rows.begin(); row_it != result.rows.end(); ++row_it) {
      bool first_el = true;
      for (auto el_it = (*row_it).begin(); el_it != (*row_it).end(); ++el_it) {
        if (!first_el)
          std::cout << " ";
        std::cout << *el_it;
        first_el = false;
      }
      std::cout << "\n";
    }

    bool first_sum = true;
    for (auto sum_it = result.sums.begin(); sum_it != result.sums.end(); ++sum_it) {
      if (!first_sum)
        std::cout << " ";
      std::cout << *sum_it;
      first_sum = false;
    }
    if (!result.sums.empty()) {
      std::cout << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
