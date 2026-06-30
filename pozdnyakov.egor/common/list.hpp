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

      template< class... Args >
      Node(Args &&...args):
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
      LIter temp = *this;
      if (ptr_) {
        ptr_ = ptr_->next_;
      }
      return temp;
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
      LCIter temp = *this;
      if (ptr_) {
        ptr_ = ptr_->next_;
      }
      return temp;
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
    List():
      fakeNode(new detail::BaseNode())
    {
      fakeNode->next_ = fakeNode;
    }

    List(const List &other):
      fakeNode(new detail::BaseNode())
    {
      fakeNode->next_ = fakeNode;
      LIter< T > tailIter = end();
      for (auto it = other.cbegin(); it != other.cend(); ++it) {
        if (empty()) {
          pushFront(*it);
          tailIter = begin();
        } else {
          insertAfter(tailIter, *it);
          ++tailIter;
        }
      }
    }

    List(List &&other) noexcept:
      fakeNode(other.fakeNode)
    {
      other.fakeNode = new detail::BaseNode();
      other.fakeNode->next_ = other.fakeNode;
    }

    List &operator=(List other)
    {
      std::swap(fakeNode, other.fakeNode);
      return *this;
    }

    ~List()
    {
      clear();
      delete fakeNode;
    }

    void pushFront(const T &val)
    {
      detail::Node< T > *newNode = new detail::Node< T >(val);
      newNode->next_ = fakeNode->next_;
      fakeNode->next_ = newNode;
    }

    void pushFront(T &&val)
    {
      detail::Node< T > *newNode = new detail::Node< T >(std::move(val));
      newNode->next_ = fakeNode->next_;
      fakeNode->next_ = newNode;
    }

    template< class... Args >
    void emplaceFront(Args &&...args)
    {
      detail::Node< T > *newNode = new detail::Node< T >(std::forward< Args >(args)...);
      newNode->next_ = fakeNode->next_;
      fakeNode->next_ = newNode;
    }

    void popFront() noexcept
    {
      if (!empty()) {
        detail::BaseNode *temp = fakeNode->next_;
        fakeNode->next_ = temp->next_;
        delete static_cast< detail::Node< T > * >(temp);
      }
    }

    void insertAfter(LIter< T > pos, const T &val)
    {
      if (pos.ptr_) {
        detail::Node< T > *newNode = new detail::Node< T >(val);
        newNode->next_ = pos.ptr_->next_;
        pos.ptr_->next_ = newNode;
      }
    }

    void insertAfter(LIter< T > pos, T &&val)
    {
      if (pos.ptr_) {
        detail::Node< T > *newNode = new detail::Node< T >(std::move(val));
        newNode->next_ = pos.ptr_->next_;
        pos.ptr_->next_ = newNode;
      }
    }

    template< class... Args >
    void emplaceAfter(LIter< T > pos, Args &&...args)
    {
      if (pos.ptr_) {
        detail::Node< T > *newNode = new detail::Node< T >(std::forward< Args >(args)...);
        newNode->next_ = pos.ptr_->next_;
        pos.ptr_->next_ = newNode;
      }
    }

    void eraseAfter(LIter< T > pos) noexcept
    {
      if (pos.ptr_ && pos.ptr_->next_ != fakeNode) {
        detail::BaseNode *temp = pos.ptr_->next_;
        pos.ptr_->next_ = temp->next_;
        delete static_cast< detail::Node< T > * >(temp);
      }
    }

    void makeCircular() noexcept
    {
      if (!empty()) {
        detail::BaseNode *current = fakeNode;
        while (current->next_ && current->next_ != fakeNode && current->next_ != nullptr) {
          current = current->next_;
        }
        current->next_ = fakeNode;
      }
    }

    bool checkCircular() const noexcept
    {
      if (empty()) {
        return false;
      }
      detail::BaseNode *slow = fakeNode->next_;
      detail::BaseNode *fast = fakeNode->next_;
      while (fast && fast->next_) {
        slow = slow->next_;
        fast = fast->next_->next_;
        if (slow == fast) {
          return slow == fakeNode;
        }
      }
      return false;
    }

    template< class Compare >
    detail::BaseNode *mergeLists(detail::BaseNode *first1, detail::BaseNode *first2, Compare comp) noexcept
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

    template< class Compare >
    void sort(Compare comp)
    {
      if (empty() || fakeNode->next_->next_ == fakeNode) {
        return;
      }

      detail::BaseNode *current = fakeNode->next_;
      while (current->next_ != fakeNode && current->next_ != nullptr) {
        current = current->next_;
      }
      current->next_ = nullptr;

      fakeNode->next_ = mergeSort(fakeNode->next_, comp);
      makeCircular();
    }

    template< class Predicate >
    void remove_if(Predicate predicate)
    {
      detail::BaseNode dummyTrue;
      detail::BaseNode *trueTail = &dummyTrue;

      detail::BaseNode dummyFalse;
      detail::BaseNode *falseTail = &dummyFalse;

      detail::BaseNode *current = fakeNode->next_;
      while (current && current != fakeNode) {
        if (predicate(static_cast< detail::Node< T > * >(current)->data_)) {
          trueTail->next_ = current;
          trueTail = trueTail->next_;
        } else {
          falseTail->next_ = current;
          falseTail = falseTail->next_;
        }
        current = current->next_;
      }

      trueTail->next_ = nullptr;
      falseTail->next_ = nullptr;

      if (dummyFalse.next_) {
        trueTail->next_ = dummyFalse.next_;
      }

      fakeNode->next_ = dummyTrue.next_ ? dummyTrue.next_ : dummyFalse.next_;
      makeCircular();
    }

    void clear() noexcept
    {
      while (!empty()) {
        popFront();
      }
    }

    bool empty() const noexcept
    {
      return fakeNode == nullptr || fakeNode->next_ == fakeNode;
    }

    T &front()
    {
      return static_cast< detail::Node< T > * >(fakeNode->next_)->data_;
    }

    const T &front() const
    {
      return static_cast< const detail::Node< T > * >(fakeNode->next_)->data_;
    }

    LIter< T > begin()
    {
      return LIter< T >(fakeNode ? fakeNode->next_ : nullptr);
    }

    LIter< T > end()
    {
      return LIter< T >(fakeNode);
    }

    LCIter< T > cbegin() const
    {
      return LCIter< T >(fakeNode ? fakeNode->next_ : nullptr);
    }

    LCIter< T > cend() const
    {
      return LCIter< T >(fakeNode);
    }

    LCIter< T > begin() const
    {
      return cbegin();
    }

    LCIter< T > end() const
    {
      return cend();
    }

  private:
    detail::BaseNode *fakeNode;
  };

}
#endif
