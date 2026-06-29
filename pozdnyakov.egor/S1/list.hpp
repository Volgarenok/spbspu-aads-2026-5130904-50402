#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <iterator>
#include <utility>

namespace pozdnyakov
{

  namespace detail
  {

    struct BaseNode
    {
      BaseNode *next_;

      BaseNode():
        next_(nullptr)
      {}

      virtual ~BaseNode() = default;
    };

    template< class T >
    struct Node: BaseNode
    {
      T data_;

      Node(const T &value):
        BaseNode(),
        data_(value)
      {}

      Node(T &&value):
        BaseNode(),
        data_(std::move(value))
      {}
    };

  }

  template< class T >
  class List;
  template< class T >
  class LCIter;

  template< class T >
  class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    LIter():
      ptr_(nullptr)
    {}

    reference operator*() const
    {
      return static_cast< detail::Node< T > * >(ptr_)->data_;
    }

    pointer operator->() const
    {
      return &(static_cast< detail::Node< T > * >(ptr_)->data_);
    }

    LIter &operator++()
    {
      if (ptr_) {
        ptr_ = ptr_->next_;
      }
      return *this;
    }

    LIter operator++(int)
    {
      const LIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter &other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LIter &other) const
    {
      return ptr_ != other.ptr_;
    }

  private:
    detail::BaseNode *ptr_;

    explicit LIter(detail::BaseNode *p):
      ptr_(p)
    {}
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    LCIter():
      ptr_(nullptr)
    {}

    LCIter(const LIter< T > &other):
      ptr_(other.ptr_)
    {}

    reference operator*() const
    {
      return static_cast< const detail::Node< T > * >(ptr_)->data_;
    }

    pointer operator->() const
    {
      return &(static_cast< const detail::Node< T > * >(ptr_)->data_);
    }

    LCIter &operator++()
    {
      if (ptr_) {
        ptr_ = ptr_->next_;
      }
      return *this;
    }

    LCIter operator++(int)
    {
      const LCIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LCIter &other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LCIter &other) const
    {
      return ptr_ != other.ptr_;
    }

  private:
    const detail::BaseNode *ptr_;

    explicit LCIter(const detail::BaseNode *p):
      ptr_(p)
    {}
  };

  template< class T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List():
      fakeNode_(new detail::BaseNode()),
      tail_(fakeNode_)
    {
      fakeNode_->next_ = fakeNode_;
    }

    ~List() noexcept
    {
      clear();
      delete fakeNode_;
    }

    List(const List &other):
      fakeNode_(new detail::BaseNode()),
      tail_(fakeNode_)
    {
      fakeNode_->next_ = fakeNode_;
      try {
        for (auto it = other.cbegin(); it != other.cend(); ++it) {
          pushBack(*it);
        }
      } catch (...) {
        clear();
        delete fakeNode_;
        throw;
      }
    }

    List(List &&other) noexcept:
      fakeNode_(other.fakeNode_),
      tail_(other.tail_)
    {
      other.fakeNode_ = new detail::BaseNode();
      other.fakeNode_->next_ = other.fakeNode_;
      other.tail_ = other.fakeNode_;
    }

    List &operator=(const List &other)
    {
      if (this != &other) {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List &operator=(List &&other) noexcept
    {
      if (this != &other) {
        List tmp(std::move(other));
        swap(tmp);
      }
      return *this;
    }

    void swap(List &other) noexcept
    {
      std::swap(fakeNode_, other.fakeNode_);
      std::swap(tail_, other.tail_);
    }

    bool empty() const noexcept
    {
      return fakeNode_->next_ == fakeNode_;
    }

    T &front()
    {
      return static_cast< detail::Node< T > * >(fakeNode_->next_)->data_;
    }

    const T &front() const
    {
      return static_cast< const detail::Node< T > * >(fakeNode_->next_)->data_;
    }

    T &back()
    {
      return static_cast< detail::Node< T > * >(tail_)->data_;
    }

    const T &back() const
    {
      return static_cast< const detail::Node< T > * >(tail_)->data_;
    }

    iterator begin()
    {
      return iterator(fakeNode_->next_);
    }

    iterator end()
    {
      return iterator(fakeNode_);
    }

    const_iterator cbegin() const
    {
      return const_iterator(fakeNode_->next_);
    }

    const_iterator cend() const
    {
      return const_iterator(fakeNode_);
    }

    void pushFront(const T &value)
    {
      detail::Node< T > *newNode = new detail::Node< T >(value);
      newNode->next_ = fakeNode_->next_;
      fakeNode_->next_ = newNode;
      if (tail_ == fakeNode_) {
        tail_ = newNode;
      }
    }

    void pushFront(T &&value)
    {
      detail::Node< T > *newNode = new detail::Node< T >(std::move(value));
      newNode->next_ = fakeNode_->next_;
      fakeNode_->next_ = newNode;
      if (tail_ == fakeNode_) {
        tail_ = newNode;
      }
    }

    void pushBack(const T &value)
    {
      detail::Node< T > *newNode = new detail::Node< T >(value);
      newNode->next_ = fakeNode_;
      tail_->next_ = newNode;
      tail_ = newNode;
    }

    void pushBack(T &&value)
    {
      detail::Node< T > *newNode = new detail::Node< T >(std::move(value));
      newNode->next_ = fakeNode_;
      tail_->next_ = newNode;
      tail_ = newNode;
    }

    iterator insertAfter(const_iterator pos, const T &value)
    {
      detail::BaseNode *posNode = const_cast< detail::BaseNode * >(pos.ptr_);
      detail::Node< T > *newNode = new detail::Node< T >(value);
      newNode->next_ = posNode->next_;
      posNode->next_ = newNode;
      if (tail_ == posNode) {
        tail_ = newNode;
      }
      return iterator(newNode);
    }

    iterator insertAfter(const_iterator pos, T &&value)
    {
      detail::BaseNode *posNode = const_cast< detail::BaseNode * >(pos.ptr_);
      detail::Node< T > *newNode = new detail::Node< T >(std::move(value));
      newNode->next_ = posNode->next_;
      posNode->next_ = newNode;
      if (tail_ == posNode) {
        tail_ = newNode;
      }
      return iterator(newNode);
    }

    void popFront() noexcept
    {
      if (!empty()) {
        detail::BaseNode *temp = fakeNode_->next_;
        fakeNode_->next_ = temp->next_;
        if (tail_ == temp) {
          tail_ = fakeNode_;
        }
        delete static_cast< detail::Node< T > * >(temp);
      }
    }

    void popBack() noexcept
    {
      if (!empty()) {
        detail::BaseNode *prev = getPrevious(tail_);
        delete static_cast< detail::Node< T > * >(tail_);
        tail_ = prev;
        tail_->next_ = fakeNode_;
      }
    }

    void clear() noexcept
    {
      while (!empty()) {
        popFront();
      }
    }

    void splice(const_iterator pos, List &other) noexcept
    {
      if (other.empty() || this == &other) {
        return;
      }
      splice(pos, other, other.cbegin(), other.cend());
    }

    void splice(const_iterator pos, List &other, const_iterator it) noexcept
    {
      if (it == other.cend()) {
        return;
      }
      const_iterator nextIt = it;
      ++nextIt;
      splice(pos, other, it, nextIt);
    }

    void splice(const_iterator pos, List &other,
      const_iterator first, const_iterator last) noexcept
    {
      if (first == last) {
        return;
      }
      if (this == &other && (pos == first || isInRange(first, last, pos))) {
        return;
      }

      detail::BaseNode *posPrev = getPrevious(pos.ptr_);
      detail::BaseNode *firstPrev = other.getPrevious(first.ptr_);
      detail::BaseNode *lastNode = getPreviousOf(
        const_cast< detail::BaseNode * >(last.ptr_),
        const_cast< detail::BaseNode * >(first.ptr_)
      );

      firstPrev->next_ = const_cast< detail::BaseNode * >(last.ptr_);

      if (other.tail_ == lastNode) {
        other.tail_ = firstPrev == other.fakeNode_ ? other.fakeNode_ : firstPrev;
      }

      lastNode->next_ = posPrev->next_;
      posPrev->next_ = const_cast< detail::BaseNode * >(first.ptr_);

      if (posPrev == tail_) {
        tail_ = lastNode;
      }
    }

    template< class Compare >
    void merge(List &other, Compare comp) noexcept
    {
      if (this == &other || other.empty()) {
        return;
      }

      makeLinear();
      other.makeLinear();

      detail::BaseNode *merged = mergeLists(fakeNode_->next_, other.fakeNode_->next_, comp);
      fakeNode_->next_ = merged;

      detail::BaseNode *cur = fakeNode_;
      while (cur->next_) {
        cur = cur->next_;
      }
      tail_ = cur;
      tail_->next_ = fakeNode_;

      other.fakeNode_->next_ = other.fakeNode_;
      other.tail_ = other.fakeNode_;
    }

    void merge(List &other) noexcept
    {
      merge(other, [](const T &a, const T &b)
      {
        return a < b;
      });
    }

    template< class Compare >
    void sort(Compare comp) noexcept
    {
      if (empty() || fakeNode_->next_->next_ == fakeNode_) {
        return;
      }

      makeLinear();
      fakeNode_->next_ = mergeSort(fakeNode_->next_, comp);

      detail::BaseNode *cur = fakeNode_;
      while (cur->next_) {
        cur = cur->next_;
      }
      tail_ = cur;
      tail_->next_ = fakeNode_;
    }

    void sort() noexcept
    {
      sort([](const T &a, const T &b)
      {
        return a < b;
      });
    }

    template< class Predicate >
    void partition(Predicate predicate) noexcept
    {
      if (empty()) {
        return;
      }

      makeLinear();

      detail::BaseNode dummyTrue;
      detail::BaseNode *trueTail = &dummyTrue;

      detail::BaseNode dummyFalse;
      detail::BaseNode *falseTail = &dummyFalse;

      detail::BaseNode *current = fakeNode_->next_;
      while (current) {
        detail::BaseNode *nextNode = current->next_;
        if (predicate(static_cast< detail::Node< T > * >(current)->data_)) {
          trueTail->next_ = current;
          trueTail = current;
        } else {
          falseTail->next_ = current;
          falseTail = current;
        }
        current = nextNode;
      }

      trueTail->next_ = nullptr;
      falseTail->next_ = nullptr;

      if (dummyTrue.next_) {
        fakeNode_->next_ = dummyTrue.next_;
        trueTail->next_ = dummyFalse.next_;
        if (dummyFalse.next_) {
          tail_ = falseTail;
          falseTail->next_ = fakeNode_;
        } else {
          tail_ = trueTail;
          trueTail->next_ = fakeNode_;
        }
      } else {
        fakeNode_->next_ = dummyFalse.next_;
        tail_ = falseTail;
        falseTail->next_ = fakeNode_;
      }
    }

  private:
    detail::BaseNode *fakeNode_;
    detail::BaseNode *tail_;

    detail::BaseNode *getPrevious(const detail::BaseNode *node) const noexcept
    {
      detail::BaseNode *current = fakeNode_;
      while (current->next_ != node) {
        current = current->next_;
      }
      return current;
    }

    detail::BaseNode *getPreviousOf(
      detail::BaseNode *target,
      detail::BaseNode *start
    ) const noexcept
    {
      detail::BaseNode *current = start;
      while (current->next_ != target) {
        current = current->next_;
      }
      return current;
    }

    bool isInRange(
      const_iterator first,
      const_iterator last,
      const_iterator needle
    ) const noexcept
    {
      for (auto it = first; it != last; ++it) {
        if (it == needle) {
          return true;
        }
      }
      return false;
    }

    void makeLinear() noexcept
    {
      if (!empty()) {
        tail_->next_ = nullptr;
      }
    }

    template< class Compare >
    detail::BaseNode *mergeLists(
      detail::BaseNode *first1,
      detail::BaseNode *first2,
      Compare comp
    ) noexcept
    {
      detail::BaseNode dummy;
      detail::BaseNode *current = &dummy;

      while (first1 && first2) {
        const T &val1 = static_cast< detail::Node< T > * >(first1)->data_;
        const T &val2 = static_cast< detail::Node< T > * >(first2)->data_;
        if (comp(val2, val1)) {
          current->next_ = first2;
          first2 = first2->next_;
        } else {
          current->next_ = first1;
          first1 = first1->next_;
        }
        current = current->next_;
      }

      current->next_ = first1 ? first1 : first2;
      return dummy.next_;
    }

    template< class Compare >
    detail::BaseNode *mergeSort(detail::BaseNode *head, Compare comp) noexcept
    {
      if (!head || !head->next_) {
        return head;
      }

      detail::BaseNode *slow = head;
      detail::BaseNode *fast = head->next_;

      while (fast && fast->next_) {
        slow = slow->next_;
        fast = fast->next_->next_;
      }

      detail::BaseNode *mid = slow->next_;
      slow->next_ = nullptr;

      detail::BaseNode *left = mergeSort(head, comp);
      detail::BaseNode *right = mergeSort(mid, comp);

      return mergeLists(left, right, comp);
    }
  };

}

#endif
