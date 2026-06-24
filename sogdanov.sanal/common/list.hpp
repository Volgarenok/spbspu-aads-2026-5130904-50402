#ifndef LIST_HPP
#define LIST_HPP

#include <functional>
#include <cstddef>
#include <utility>

namespace sogdanov {

  template< class T >
  struct Node {
    Node(const T &d, Node *n);
    Node(T &&val, Node *n);
    template< class... Args >
    Node(Node *n, Args &&... args);
    T data;
    Node *next;
  };

  template< class T >
  class List;

  template< class T >
  class LCIter;

  template< class T >
  class LIter {
  public:
    LIter() noexcept;
    LIter(Node< T > *p) noexcept;

    T &operator*() const;
    LIter &operator++() noexcept;
    bool operator==(const LIter &o) const noexcept;
    bool operator!=(const LIter &o) const noexcept;

  private:
    friend class List< T >;
    friend class LCIter< T >;

    Node< T > *ptr_;
  };

  template< class T >
  class LCIter {
  public:
    LCIter() noexcept;
    LCIter(const Node< T > *p) noexcept;
    LCIter(const LIter< T > &it) noexcept;

    const T &operator*() const;
    LCIter &operator++() noexcept;
    bool operator==(const LCIter &o) const noexcept;
    bool operator!=(const LCIter &o) const noexcept;

  private:
    friend class List< T >;
    friend class LIter< T >;

    const Node< T > *ptr_;
  };

  template< class T >
  class List {
  public:
    List() noexcept;
    List(const List &other);
    List(List &&other) noexcept;
    List &operator=(const List &other);
    List &operator=(List &&other) noexcept;
    ~List() noexcept;

    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > end() const noexcept;

    void push_front(const T &val);
    template< class... Args >
    void emplace_front(Args &&... args);
    template< class... Args >
    void emplace_back(Args &&... args);
    void pop_front();
    LIter< T > insert_after(LIter< T > pos, const T &val);
    LIter< T > insert_after(LIter< T > pos, T &&val);

    void clear() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;

    void splice_after(LIter< T > pos, List< T > &other) noexcept;
    void splice_after(LIter< T > pos, List< T > &other, LIter< T > it) noexcept;
    void splice_after(LIter< T > pos, List< T > &other,
        LIter< T > first, LIter< T > last) noexcept;
    template< class Compare = std::less< T > >
    void merge(List< T > &other, Compare comp = Compare()) noexcept;
    template< class Compare = std::less< T > >
    void sort(Compare comp = Compare()) noexcept;
    template< class Pred >
    List< T > partition(Pred pred) noexcept;
  private:
    Node< T > *head_;
    Node< T > *tail_;
    size_t size_;
    void splice_chain(Node< T > *pos_node, List< T > &other,
        Node< T > *prev_node, Node< T > *range_begin,
        Node< T > *range_end, size_t count) noexcept;
    List< T > split_half() noexcept;
  };

  template< class T >
  Node< T >::Node(const T &d, Node *n):
    data(d),
    next(n)
  {
  }

  template< class T >
  Node< T >::Node(T &&val, Node *n):
    data(std::move(val)),
    next(n)
  {
  }
  template< class T >
  template< class... Args >
  Node< T >::Node(Node *n, Args &&... args):
    data(std::forward< Args >(args)...),
    next(n)
  {}
  template< class T >
  LIter< T >::LIter() noexcept:
    ptr_(nullptr)
  {
  }

  template< class T >
  LIter< T >::LIter(Node< T > *p) noexcept:
    ptr_(p)
  {
  }

  template< class T >
  T &LIter< T >::operator*() const
  {
    return ptr_->data;
  }

  template< class T >
  LIter< T > &LIter< T >::operator++() noexcept
  {
    if (ptr_) {
      ptr_ = ptr_->next;
    }
    return *this;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter &o) const noexcept
  {
    return ptr_ == o.ptr_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter &o) const noexcept
  {
    return ptr_ != o.ptr_;
  }

  template< class T >
  LCIter< T >::LCIter() noexcept:
    ptr_(nullptr)
  {
  }

  template< class T >
  LCIter< T >::LCIter(const Node< T > *p) noexcept:
    ptr_(p)
  {
  }

  template< class T >
  LCIter< T >::LCIter(const LIter< T > &it) noexcept:
    ptr_(it.ptr_)
  {
  }

  template< class T >
  const T &LCIter< T >::operator*() const
  {
    return ptr_->data;
  }

  template< class T >
  LCIter< T > &LCIter< T >::operator++() noexcept
  {
    if (ptr_) {
      ptr_ = ptr_->next;
    }
    return *this;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter &o) const noexcept
  {
    return ptr_ == o.ptr_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter &o) const noexcept
  {
    return ptr_ != o.ptr_;
  }

  template< class T >
  List< T >::List() noexcept:
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
  }

  template< class T >
  List< T >::List(const List &other):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    try {
      for (LCIter< T > it = other.begin(); it != other.end(); ++it) {
        Node< T > *node = new Node< T >(*it, nullptr);
        if (!tail_) {
          head_ = node;
          tail_ = node;
        } else {
          tail_->next = node;
          tail_ = node;
        }
        ++size_;
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::List(List &&other) noexcept:
    head_(other.head_),
    tail_(other.tail_),
    size_(other.size_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  List< T > &List< T >::operator=(const List &other)
  {
    if (this != &other) {
      List tmp(other);
      std::swap(head_, tmp.head_);
      std::swap(tail_, tmp.tail_);
      std::swap(size_, tmp.size_);
    }
    return *this;
  }

  template< class T >
  List< T > &List< T >::operator=(List &&other) noexcept
  {
    if (this != &other) {
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

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >(head_);
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >(nullptr);
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >(head_);
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >(nullptr);
  }

  template< class T >
  void List< T >::push_front(const T &val)
  {
    Node< T > *node = new Node< T >(val, head_);
    head_ = node;
    if (!tail_) {
      tail_ = head_;
    }
    ++size_;
  }
  template< class T >
  template< class... Args >
  void List< T >::emplace_front(Args &&... args)
  {
    Node< T > *node = new Node< T >(head_, std::forward< Args >(args)...);
    head_ = node;
    if (!tail_) {
      tail_ = head_;
    }
    ++size_;
  }

  template< class T >
  template< class... Args >
  void List< T >::emplace_back(Args &&... args)
  {
    Node< T > *node = new Node< T >(nullptr, std::forward< Args >(args)...);
    if (!tail_) {
      head_ = tail_ = node;
    }
    else {
      tail_->next = node;
      tail_ = node;
    }
    ++size_;
  }

  template< class T >
  void List< T >::pop_front()
  {
    if (empty()) {
      return;
    }
    Node< T > *old = head_;
    head_ = head_->next;
    delete old;
    --size_;
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > pos, const T &val)
  {
    if (!pos.ptr_) {
      return end();
    }
    Node< T > *node = new Node< T >(val, pos.ptr_->next);
    pos.ptr_->next = node;
    if (pos.ptr_ == tail_) {
      tail_ = node;
    }
    ++size_;
    return LIter< T >(node);
  }

  template< class T >
  LIter< T > List< T >::insert_after(LIter< T > pos, T &&val)
  {
    if (!pos.ptr_) {
      return end();
    }
    Node< T > *node = new Node< T >(std::move(val), pos.ptr_->next);
    pos.ptr_->next = node;
    if (pos.ptr_ == tail_) {
      tail_ = node;
    }
    ++size_;
    return LIter< T >(node);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    Node< T > *cur = head_;
    while (cur) {
      Node< T > *n = cur->next;
      delete cur;
      cur = n;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return head_ == nullptr;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void List< T >::splice_chain(Node< T > *pos_node, List< T > &other,
      Node< T > *prev_node, Node< T > *range_begin,
      Node< T > *range_end, size_t count) noexcept
  {
    Node< T > *after_range = range_end->next;
    if (prev_node) {
      prev_node->next = after_range;
      if (other.tail_ == range_end) {
        other.tail_ = prev_node;
      }
    } else {
      other.head_ = after_range;
      if (!after_range) {
        other.tail_ = nullptr;
      }
    }
    other.size_ -= count;
    range_end->next = pos_node->next;
    pos_node->next = range_begin;
    if (tail_ == pos_node) {
      tail_ = range_end;
    }
    size_ += count;
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List< T > &other) noexcept
  {
    if (&other == this || other.empty() || !pos.ptr_) {
      return;
    }
    splice_chain(pos.ptr_, other, nullptr, other.head_, other.tail_, other.size_);
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List< T > &other,
      LIter< T > it) noexcept
  {
    if (&other == this || !pos.ptr_ || !it.ptr_ || !it.ptr_->next) {
      return;
    }
    Node< T > *to_move = it.ptr_->next;
    splice_chain(pos.ptr_, other, it.ptr_, to_move, to_move, 1);
  }

  template< class T >
  void List< T >::splice_after(LIter< T > pos, List< T > &other,
      LIter< T > first, LIter< T > last) noexcept
  {
    if (&other == this || !pos.ptr_ || !first.ptr_
        || first.ptr_->next == last.ptr_) {
      return;
    }
    Node< T > *range_end = first.ptr_->next;
    size_t count = 1;
    while (range_end->next != last.ptr_) {
      range_end = range_end->next;
      ++count;
    }
    splice_chain(pos.ptr_, other, first.ptr_, first.ptr_->next, range_end, count);
  }

  template< class T >
  template< class Compare >
  void List< T >::merge(List< T > &other, Compare comp) noexcept
  {
    if (other.empty()) {
      return;
    }
    if (empty()) {
      *this = std::move(other);
      return;
    }
    Node< T > *a = head_;
    Node< T > *b = other.head_;
    Node< T > *result_head;
    if (!comp(b->data, a->data)) {
      result_head = a;
      a = a->next;
    } else {
      result_head = b;
      b = b->next;
    }
    Node< T > *result_tail = result_head;
    while (a && b) {
      if (!comp(b->data, a->data)) {
        result_tail->next = a;
        a = a->next;
      } else {
        result_tail->next = b;
        b = b->next;
      }
      result_tail = result_tail->next;
    }
    if (a) {
      result_tail->next = a;
    } else {
      result_tail->next = b;
      tail_ = other.tail_;
    }
    head_ = result_head;
    size_ += other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  List< T > List< T >::split_half() noexcept
  {
    if (size_ <= 1) {
      return List< T >();
    }
    size_t half = size_ / 2;
    Node< T > *cur = head_;
    for (size_t i = 1; i < half; ++i) {
      cur = cur->next;
    }
    List< T > second;
    second.head_ = cur->next;
    second.tail_ = tail_;
    second.size_ = size_ - half;
    cur->next = nullptr;
    tail_ = cur;
    size_ = half;
    return second;
  }

  template< class T >
  template< class Compare >
  void List< T >::sort(Compare comp) noexcept
  {
    if (size_ <= 1) {
      return;
    }
    List< T > second = split_half();
    sort(comp);
    second.sort(comp);
    merge(second, comp);
  }

  template< class T >
  template< class Pred >
  List< T > List< T >::partition(Pred pred) noexcept
  {
    Node< T > *acc_head = nullptr;
    Node< T > *acc_tail = nullptr;
    Node< T > *rej_head = nullptr;
    Node< T > *rej_tail = nullptr;
    size_t acc_size = 0;
    size_t rej_size = 0;
    Node< T > *cur = head_;
    while (cur) {
      Node< T > *next = cur->next;
      cur->next = nullptr;
      if (pred(cur->data)) {
        if (acc_tail) {
          acc_tail->next = cur;
        } else {
          acc_head = cur;
        }
        acc_tail = cur;
        ++acc_size;
      } else {
        if (rej_tail) {
          rej_tail->next = cur;
        } else {
          rej_head = cur;
        }
        rej_tail = cur;
        ++rej_size;
      }
      cur = next;
    }
    head_ = acc_head;
    tail_ = acc_tail;
    size_ = acc_size;
    List< T > rejected;
    rejected.head_ = rej_head;
    rejected.tail_ = rej_tail;
    rejected.size_ = rej_size;
    return rejected;
  }
}

#endif
