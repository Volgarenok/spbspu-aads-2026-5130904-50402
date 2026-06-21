#ifndef LIST_HPP
#define LIST_HPP

#include <utility>
#include <cstddef>
#include <memory>
#include <utility>
#include <functional>

namespace petrov {

  template< class T >
  class List;

  template< class T >
  struct Node
  {
    T value_;
    Node< T >* next_;
    Node< T >* prev_;

    Node(const T& value):
      value_(value),
      next_(nullptr),
      prev_(nullptr)
    {}
  };

  template< class T >
  class LIter
  {
    friend class List< T >;
  public:
    LIter(Node< T >* ptr = nullptr):
      ptr_(ptr)
    {}

    T& operator*() const
    {
      return ptr_->value_;
    }

    T* operator->() const
    {
      return std::addressof(ptr_->value_);
    }

    LIter< T >& operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }

    LIter< T > operator++(int)
    {
      LIter< T > temp(*this);
      ++(*this);
      return temp;
    }

    LIter< T >& operator--()
    {
      ptr_ = ptr_->prev_;
      return *this;
    }

    LIter< T > operator--(int)
    {
      LIter< T > temp(*this);
      --(*this);
      return temp;
    }

    bool operator==(const LIter< T >& other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LIter< T >& other) const
    {
      return !(*this == other);
    }

  private:
    Node< T >* ptr_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
  public:
    LCIter(const Node< T >* ptr = nullptr):
      ptr_(ptr)
    {}

    LCIter(const LIter< T >& other):
      ptr_(other.ptr_)
    {}

    const T& operator*() const
    {
      return ptr_->value_;
    }

    const T* operator->() const
    {
      return std::addressof(ptr_->value_);
    }

    LCIter< T >& operator++()
    {
      ptr_ = ptr_->next_;
      return *this;
    }

    LCIter< T > operator++(int)
    {
      LCIter< T > temp(*this);
      ++(*this);
      return temp;
    }

    LCIter< T >& operator--()
    {
      ptr_ = ptr_->prev_;
      return *this;
    }

    LCIter< T > operator--(int)
    {
      LCIter< T > temp(*this);
      --(*this);
      return temp;
    }

    bool operator==(const LCIter< T >& other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LCIter< T >& other) const
    {
      return !(*this == other);
    }

  private:
    const Node< T >* ptr_;
  };

  template< class T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List():
      head_(nullptr),
      tail_(nullptr),
      size_(0)
    {}

    List(const List< T >& other):
      head_(nullptr),
      tail_(nullptr),
      size_(0)
    {
      try {
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
          push_back(*it);
        }
      } catch (...) {
        clear();
        throw;
      }
    }

    List(List< T >&& other) noexcept:
      head_(other.head_),
      tail_(other.tail_),
      size_(other.size_)
    {
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }

    ~List()
    {
      clear();
    }

    List< T >& operator=(const List< T >& other)
    {
      if (this != std::addressof(other)) {
        List< T > temp(other);
        swap(temp);
      }
      return *this;
    }

    List< T >& operator=(List< T >&& other) noexcept
    {
      if (this != std::addressof(other)) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    iterator begin()
    {
      return iterator(head_);
    }

    iterator end()
    {
      return iterator(nullptr);
    }

    const_iterator begin() const
    {
      return cbegin();
    }

    const_iterator end() const
    {
      return cend();
    }

    const_iterator cbegin() const
    {
      return const_iterator(head_);
    }

    const_iterator cend() const
    {
      return const_iterator(nullptr);
    }

    T& front()
    {
      return head_->value_;
    }

    const T& front() const
    {
      return head_->value_;
    }

    T& back()
    {
      return tail_->value_;
    }

    const T& back() const
    {
      return tail_->value_;
    }

    void push_front(const T& value)
    {
      Node< T >* new_node = new Node< T >(value);
      new_node->next_ = head_;
      if (head_ != nullptr) {
        head_->prev_ = new_node;
      } else {
        tail_ = new_node;
      }
      head_ = new_node;
      ++size_;
    }

    void push_back(const T& value)
    {
      Node< T >* new_node = new Node< T >(value);
      new_node->prev_ = tail_;
      if (tail_ != nullptr) {
        tail_->next_ = new_node;
      } else {
        head_ = new_node;
      }
      tail_ = new_node;
      ++size_;
    }

    void popFront()
    {
      if (head_ == nullptr) {
        return;
      }
      Node< T >* temp = head_;
      head_ = head_->next_;
      if (head_ != nullptr) {
        head_->prev_ = nullptr;
      } else {
        tail_ = nullptr;
      }
      delete temp;
      --size_;
    }

    void popBack()
    {
      if (tail_ == nullptr) {
        return;
      }
      Node< T >* temp = tail_;
      tail_ = tail_->prev_;
      if (tail_ != nullptr) {
        tail_->next_ = nullptr;
      } else {
        head_ = nullptr;
      }
      delete temp;
      --size_;
    }

    void clear()
    {
      while (head_ != nullptr) {
        Node< T >* temp = head_;
        head_ = head_->next_;
        delete temp;
      }
      tail_ = nullptr;
      size_ = 0;
    }

    std::size_t size() const
    {
      return size_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

    void swap(List< T >& other) noexcept
    {
      std::swap(head_, other.head_);
      std::swap(tail_, other.tail_);
      std::swap(size_, other.size_);
    }

    void splice(iterator position, List< T >& other) noexcept
    {
      if (other.empty()) {
        return;
      }
      Node< T >* posNode = position.ptr_;
      Node< T >* otherFirst = other.head_;
      Node< T >* otherLast = other.tail_;

      if (posNode == nullptr) {
        otherFirst->prev_ = tail_;
        otherLast->next_ = nullptr;
        if (tail_ != nullptr) {
          tail_->next_ = otherFirst;
        } else {
          head_ = otherFirst;
        }
        tail_ = otherLast;
      } else if (posNode == head_) {
        otherFirst->prev_ = nullptr;
        otherLast->next_ = head_;
        head_->prev_ = otherLast;
        head_ = otherFirst;
      } else {
        otherFirst->prev_ = posNode->prev_;
        posNode->prev_->next_ = otherFirst;
        otherLast->next_ = posNode;
        posNode->prev_ = otherLast;
      }

      size_ += other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }

    void splice(iterator position, List< T >&& other) noexcept
    {
      splice(position, other);
    }

    void splice(iterator position, List< T >& other, iterator i) noexcept
    {
      if (i == other.end()) {
        return;
      }
      Node< T >* posNode = position.ptr_;
      Node< T >* node = i.ptr_;

      if (node == other.head_) {
        other.head_ = node->next_;
      } else {
        node->prev_->next_ = node->next_;
      }
      if (node == other.tail_) {
        other.tail_ = node->prev_;
      } else {
        node->next_->prev_ = node->prev_;
      }
      --other.size_;

      if (posNode == nullptr) {
        node->prev_ = tail_;
        node->next_ = nullptr;
        if (tail_ != nullptr) {
          tail_->next_ = node;
        } else {
          head_ = node;
        }
        tail_ = node;
      } else if (posNode == head_) {
        node->prev_ = nullptr;
        node->next_ = head_;
        head_->prev_ = node;
        head_ = node;
      } else {
        node->prev_ = posNode->prev_;
        posNode->prev_->next_ = node;
        node->next_ = posNode;
        posNode->prev_ = node;
      }
      ++size_;
    }

    void splice(iterator position, List< T >&& other, iterator i) noexcept
    {
      splice(position, other, i);
    }

    void splice(iterator position, List< T >& other, iterator first, iterator last) noexcept
    {
      if (first == last) {
        return;
      }
      std::size_t count = 0;
      for (iterator it = first; it != last; ++it) {
        ++count;
      }

      Node< T >* fNode = first.ptr_;
      Node< T >* lNode = last.ptr_;
      Node< T >* rangeLast = lNode ? lNode->prev_ : other.tail_;

      if (fNode == other.head_) {
        other.head_ = lNode;
      } else {
        fNode->prev_->next_ = lNode;
      }
      if (lNode == nullptr) {
        other.tail_ = fNode->prev_;
      } else {
        lNode->prev_ = fNode->prev_;
      }
      other.size_ -= count;

      Node< T >* posNode = position.ptr_;
      if (posNode == nullptr) {
        fNode->prev_ = tail_;
        rangeLast->next_ = nullptr;
        if (tail_ != nullptr) {
          tail_->next_ = fNode;
        } else {
          head_ = fNode;
        }
        tail_ = rangeLast;
      } else if (posNode == head_) {
        fNode->prev_ = nullptr;
        rangeLast->next_ = head_;
        head_->prev_ = rangeLast;
        head_ = fNode;
      } else {
        fNode->prev_ = posNode->prev_;
        posNode->prev_->next_ = fNode;
        rangeLast->next_ = posNode;
        posNode->prev_ = rangeLast;
      }
      size_ += count;
    }

    void splice(iterator position, List< T >&& other, iterator first, iterator last) noexcept
    {
      splice(position, other, first, last);
    }

    void sort() noexcept
    {
      sort(std::less< T >{});
    }

    template< class Comparator >
    void sort(Comparator cmp) noexcept
    {
      if (size_ < 2) {
        return;
      }
      iterator mid = begin();
      for (std::size_t i = 0; i < size_ / 2; ++i) {
        ++mid;
      }
      List< T > left;
      left.splice(left.end(), *this, begin(), mid);
      left.sort(cmp);
      sort(cmp);
      merge(left, cmp);
    }

    void merge(List< T >& other) noexcept
    {
      merge(other, std::less< T >{});
    }

    template< class Comparator >
    void merge(List< T >& other, Comparator cmp) noexcept
    {
      assert(this != std::addressof(other));
      if (other.empty()) {
        return;
      }
      Node< T >* otherFirst = other.head_;
      std::size_t otherCount = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;

      Node< T >* resultHead = nullptr;
      Node< T >* resultTail = nullptr;
      Node< T >* p1 = head_;
      Node< T >* p2 = otherFirst;
      while (p1 != nullptr && p2 != nullptr) {
        Node< T >* next;
        Node< T >* chosen;
        if (cmp(p2->value_, p1->value_)) {
          chosen = p2;
          next = p2->next_;
        } else {
          chosen = p1;
          next = p1->next_;
        }
        chosen->prev_ = resultTail;
        chosen->next_ = nullptr;
        if (resultTail != nullptr) {
          resultTail->next_ = chosen;
        } else {
          resultHead = chosen;
        }
        resultTail = chosen;
        if (chosen == p2) {
          p2 = next;
        } else {
          p1 = next;
        }
      }
      Node< T >* rest = (p1 != nullptr) ? p1 : p2;
      while (rest != nullptr) {
        Node< T >* next = rest->next_;
        rest->prev_ = resultTail;
        rest->next_ = nullptr;
        if (resultTail != nullptr) {
          resultTail->next_ = rest;
        } else {
          resultHead = rest;
        }
        resultTail = rest;
        rest = next;
      }

      head_ = resultHead;
      tail_ = resultTail;
      size_ += otherCount;
    }

    void merge(List< T >&& other) noexcept
    {
      merge(other);
    }

    template< class Comparator >
    void merge(List< T > &&other, Comparator cmp) noexcept
    {
      merge(other, cmp);
    }

    template< class Predicate >
    iterator partition(Predicate pred)
    {
      List< T > falseList;
      iterator it = begin();
      while (it != end()) {
        iterator next = it;
        ++next;
        if (!pred(*it)) {
          falseList.splice(falseList.end(), *this, it);
        }
        it = next;
      }
      if (falseList.empty()) {
        return end();
      }
      iterator split = falseList.begin();
      splice(end(), falseList);
      return split;
    }

  private:
    Node< T >* head_;
    Node< T >* tail_;
    std::size_t size_;
  };

}

#endif

