#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>

namespace pozdnyakov
{

  namespace detail
  {

    struct BaseNode
    {
      BaseNode *next_;

      BaseNode() noexcept:
        next_(nullptr)
      {}
    };

    template< class T >
    struct Node: BaseNode
    {
      T data_;

      template< class... Args >
      explicit Node(Args &&...args):
        BaseNode(),
        data_(std::forward< Args >(args)...)
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

    LIter() noexcept:
      ptr_(nullptr)
    {}

    reference operator*() const
    {
      return static_cast< detail::Node< T > * >(ptr_)->data_;
    }

    pointer operator->() const
    {
      return std::addressof(static_cast< detail::Node< T > * >(ptr_)->data_);
    }

    LIter &operator++() noexcept
    {
      if (ptr_ != nullptr) {
        ptr_ = ptr_->next_;
      }
      return *this;
    }

    LIter operator++(int) noexcept
    {
      LIter temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const LIter &other) const noexcept
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LIter &other) const noexcept
    {
      return ptr_ != other.ptr_;
    }

  private:
    detail::BaseNode *ptr_;

    explicit LIter(detail::BaseNode *p) noexcept:
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

    LCIter() noexcept:
      ptr_(nullptr)
    {}

    LCIter(const LIter< T > &other) noexcept:
      ptr_(other.ptr_)
    {}

    reference operator*() const
    {
      return static_cast< const detail::Node< T > * >(ptr_)->data_;
    }

    pointer operator->() const
    {
      return std::addressof(static_cast< const detail::Node< T > * >(ptr_)->data_);
    }

    LCIter &operator++() noexcept
    {
      if (ptr_ != nullptr) {
        ptr_ = ptr_->next_;
      }
      return *this;
    }

    LCIter operator++(int) noexcept
    {
      LCIter temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const LCIter &other) const noexcept
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LCIter &other) const noexcept
    {
      return ptr_ != other.ptr_;
    }

  private:
    const detail::BaseNode *ptr_;

    explicit LCIter(const detail::BaseNode *p) noexcept:
      ptr_(p)
    {}
  };

  template< class T >
  class List
  {
  public:
    using value_type = T;
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List() noexcept:
      fakeNode_(nullptr)
    {}

    List(const List &other):
      fakeNode_(nullptr)
    {
      try {
        LIter< T > tail = end();
        for (auto it = other.cbegin(); it != other.cend(); ++it) {
          if (empty()) {
            pushFront(*it);
            tail = begin();
          } else {
            insertAfter(tail, *it);
            ++tail;
          }
        }
      } catch (...) {
        clear();
        delete fakeNode_;
        throw;
      }
    }

    List(List &&other) noexcept:
      fakeNode_(other.fakeNode_)
    {
      other.fakeNode_ = nullptr;
    }

    ~List()
    {
      clear();
      delete fakeNode_;
    }

    List &operator=(List other) noexcept
    {
      swap(other);
      return *this;
    }

    void swap(List &other) noexcept
    {
      std::swap(fakeNode_, other.fakeNode_);
    }

    void pushFront(const T &val)
    {
      emplaceFront(val);
    }

    void pushFront(T &&val)
    {
      emplaceFront(std::move(val));
    }

    template< class... Args >
    void emplaceFront(Args &&...args)
    {
      detail::BaseNode * const head = ensureSentinel();
      detail::Node< T > * const newNode = new detail::Node< T >(std::forward< Args >(args)...);
      newNode->next_ = head->next_;
      head->next_ = newNode;
    }

    void popFront() noexcept
    {
      if (!empty()) {
        detail::BaseNode * const temp = fakeNode_->next_;
        fakeNode_->next_ = temp->next_;
        delete static_cast< detail::Node< T > * >(temp);
      }
    }

    void insertAfter(LIter< T > pos, const T &val)
    {
      emplaceAfter(pos, val);
    }

    void insertAfter(LIter< T > pos, T &&val)
    {
      emplaceAfter(pos, std::move(val));
    }

    template< class... Args >
    void emplaceAfter(LIter< T > pos, Args &&...args)
    {
      if (pos.ptr_ != nullptr) {
        detail::Node< T > * const newNode = new detail::Node< T >(std::forward< Args >(args)...);
        newNode->next_ = pos.ptr_->next_;
        pos.ptr_->next_ = newNode;
      }
    }

    void eraseAfter(LIter< T > pos) noexcept
    {
      if (pos.ptr_ != nullptr && pos.ptr_->next_ != fakeNode_) {
        detail::BaseNode * const temp = pos.ptr_->next_;
        pos.ptr_->next_ = temp->next_;
        delete static_cast< detail::Node< T > * >(temp);
      }
    }

    template< class Compare >
    void sort(Compare comp)
    {
      if (empty() || fakeNode_->next_->next_ == fakeNode_) {
        return;
      }
      openCircle();
      fakeNode_->next_ = mergeSort(fakeNode_->next_, comp);
      makeCircular();
    }

    template< class Predicate >
    LIter< T > partition(Predicate predicate)
    {
      if (empty()) {
        return end();
      }

      detail::BaseNode dummyTrue;
      detail::BaseNode *trueTail = std::addressof(dummyTrue);
      detail::BaseNode dummyFalse;
      detail::BaseNode *falseTail = std::addressof(dummyFalse);

      detail::BaseNode *current = fakeNode_->next_;
      while (current != nullptr && current != fakeNode_) {
        detail::BaseNode * const next = current->next_;
        if (predicate(static_cast< detail::Node< T > * >(current)->data_)) {
          trueTail->next_ = current;
          trueTail = current;
        } else {
          falseTail->next_ = current;
          falseTail = current;
        }
        current = next;
      }

      trueTail->next_ = dummyFalse.next_;
      falseTail->next_ = nullptr;
      fakeNode_->next_ = dummyTrue.next_;
      makeCircular();

      return LIter< T >(dummyFalse.next_ != nullptr ? dummyFalse.next_ : fakeNode_);
    }

    void clear() noexcept
    {
      while (!empty()) {
        popFront();
      }
    }

    bool empty() const noexcept
    {
      return fakeNode_ == nullptr || fakeNode_->next_ == fakeNode_;
    }

    T &front()
    {
      return static_cast< detail::Node< T > * >(fakeNode_->next_)->data_;
    }

    const T &front() const
    {
      return static_cast< const detail::Node< T > * >(fakeNode_->next_)->data_;
    }

    LIter< T > begin() noexcept
    {
      return LIter< T >(fakeNode_ != nullptr ? fakeNode_->next_ : nullptr);
    }

    LIter< T > end() noexcept
    {
      return LIter< T >(fakeNode_);
    }

    LCIter< T > cbegin() const noexcept
    {
      return LCIter< T >(fakeNode_ != nullptr ? fakeNode_->next_ : nullptr);
    }

    LCIter< T > cend() const noexcept
    {
      return LCIter< T >(fakeNode_);
    }

    LCIter< T > begin() const noexcept
    {
      return cbegin();
    }

    LCIter< T > end() const noexcept
    {
      return cend();
    }

  private:
    detail::BaseNode *fakeNode_;

    detail::BaseNode *ensureSentinel()
    {
      if (fakeNode_ == nullptr) {
        fakeNode_ = new detail::BaseNode();
        fakeNode_->next_ = fakeNode_;
      }
      return fakeNode_;
    }

    void openCircle() noexcept
    {
      detail::BaseNode *current = fakeNode_->next_;
      while (current->next_ != fakeNode_ && current->next_ != nullptr) {
        current = current->next_;
      }
      current->next_ = nullptr;
    }

    void makeCircular() noexcept
    {
      detail::BaseNode *current = fakeNode_;
      while (current->next_ != nullptr && current->next_ != fakeNode_) {
        current = current->next_;
      }
      current->next_ = fakeNode_;
    }

    template< class Compare >
    detail::BaseNode *mergeLists(detail::BaseNode *first1, detail::BaseNode *first2, Compare comp)
    {
      detail::BaseNode dummy;
      detail::BaseNode *current = std::addressof(dummy);

      while (first1 != nullptr && first2 != nullptr) {
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

      current->next_ = (first1 != nullptr) ? first1 : first2;
      return dummy.next_;
    }

    template< class Compare >
    detail::BaseNode *mergeSort(detail::BaseNode *head, Compare comp)
    {
      if (head == nullptr || head->next_ == nullptr) {
        return head;
      }

      detail::BaseNode *slow = head;
      detail::BaseNode *fast = head->next_;
      while (fast != nullptr && fast->next_ != nullptr) {
        slow = slow->next_;
        fast = fast->next_->next_;
      }

      detail::BaseNode * const mid = slow->next_;
      slow->next_ = nullptr;

      detail::BaseNode * const left = mergeSort(head, comp);
      detail::BaseNode * const right = mergeSort(mid, comp);

      return mergeLists(left, right, comp);
    }
  };

}

#endif
