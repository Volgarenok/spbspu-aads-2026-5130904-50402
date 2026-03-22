#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>
namespace sogdanov {
  template<class T>
  struct Node {
    T data;
    Node* next;
    Node(const T& d, Node* n);
    Node(T&& val, Node* n);
  };
  template<class T>
  class List;
  template<class T>
  class LCIter;
  template<class T>
  class LIter {
    friend class List<T>;
    friend class LCIter<T>;
    Node<T>* ptr_;
  public:
    LIter() noexcept;
    LIter(Node<T>* p) noexcept;
    T& operator*() const;
    LIter& operator++() noexcept;
    bool operator==(const LIter& o) const noexcept;
    bool operator!=(const LIter& o) const noexcept;
  };
  template<class T>
  class LCIter {
    friend class List<T>;
    friend class LIter<T>;
    const Node<T>* ptr_;
  public:
    LCIter() noexcept;
    LCIter(const Node<T>* p) noexcept;
    LCIter(const LIter<T>& it) noexcept;
    const T& operator*() const;
    LCIter& operator++() noexcept;
    bool operator==(const LCIter& o) const noexcept;
    bool operator!=(const LCIter& o) const noexcept;
  };
  template<class T>
  class List {
    Node<T>* head_;
    Node<T>* tail_;
    size_t size_;
  public:
    List() noexcept;
    List(const List& other);
    List(List&& other) noexcept;
    List& operator=(const List& other);
    List& operator=( List&& other) noexcept;
    ~List() noexcept;
    LIter<T> begin() noexcept;
    LIter<T> end() noexcept;
    LCIter<T> begin() const noexcept;
    LCIter<T> end() const noexcept;
    void push_front(const T& val);
    void push_back(const T& val);
    void pop_front();
    LIter<T> insert_after(LIter<T> pos, const T& val);
    LIter<T> insert_after(LIter<T> pos, T&& val);
    void clear() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
  };
  template<class T>
  Node<T>::Node(const T& val, Node* n) : data(val), next(n) {}
  template<class T>
  Node<T>::Node(T&& val, Node* n) : data(std::move(val)), next(n) {}
  template<class T>
  LIter<T>::LIter() noexcept : ptr_(nullptr) {}
  template<class T>
  LIter<T>::LIter(Node<T>* p) noexcept : ptr_(p) {}
  template<class T>
  T& LIter<T>::operator*() const {
    return ptr_->data;
  }
  template<class T>
  LIter<T>& LIter<T>::operator++() noexcept {
    if (ptr_) {
      ptr_ = ptr_->next;
    }
    return *this;
  }
  template<class T>
  bool LIter<T>::operator==(const LIter& o) const noexcept {
    return ptr_ == o.ptr_;
  }
  template<class T>
  bool LIter<T>::operator!=(const LIter& o) const noexcept {
    return ptr_ != o.ptr_;
  }
  template<class T>
  LCIter<T>::LCIter() noexcept : ptr_(nullptr) {}
  template<class T>
  LCIter<T>::LCIter(const Node<T>* p) noexcept : ptr_(p) {}
  template<class T>
  LCIter<T>::LCIter(const LIter<T>& it) noexcept : ptr_(it.ptr_) {}
  template<class T>
  const T& LCIter<T>::operator*() const {
    return ptr_->data;
  }
  template<class T>
  LCIter<T>& LCIter<T>::operator++() noexcept {
    if (ptr_) {
      ptr_ = ptr_->next;
    }
    return *this;
  }
  template<class T>
  bool LCIter<T>::operator==(const LCIter& o) const noexcept {
    return ptr_ == o.ptr_;
  }
  template<class T>
  bool LCIter<T>::operator!=(const LCIter& o) const noexcept {
    return ptr_ != o.ptr_;
  }
  template<class T>
  List<T>::List() noexcept : head_(nullptr), tail_(nullptr), size_(0) {}
  template<class T>
  List<T>::List(const List& other) : head_(nullptr), tail_(nullptr), size_(0) {
    try {
      for (LCIter<T> it = other.begin(); it != other.end(); ++it) {
        Node<T>* node = new Node<T>(*it, nullptr);
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
  template<class T>
  List<T>::List(List&& other) noexcept
  : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }
  template<class T>
  List<T>& List<T>::operator=(const List& other) {
  if (this != &other) {
    List tmp(other);
    std::swap(head_, tmp.head_);
    std::swap(tail_, tmp.tail_);
    std::swap(size_, tmp.size_);
  }
  return *this;
  }
  template<class T>
  List<T>& List<T>::operator=(List&& other) noexcept {
  if (this != &other) {
    clear();
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }
  return *this;
  }
  template<class T>
  List<T>::~List() noexcept {
    clear();
  }
  template<class T>
  LIter<T> List<T>::begin() noexcept {
    return LIter<T>(head_);
  }
  template<class T>
  LIter<T> List<T>::end() noexcept {
    return LIter<T>(nullptr);
  }
  template<class T>
  LCIter<T> List<T>::begin() const noexcept {
    return LCIter<T>(head_);
  }
  template<class T>
  LCIter<T> List<T>::end() const noexcept {
    return LCIter<T>(nullptr);
  }
  template<class T>
  void List<T>::push_front(const T& val) {
    Node<T>* node = new Node<T>(val, head_);
    head_ = node;
    if (!tail_) {
      tail_ = head_;
    }
    ++size_;
  }
  template<class T>
  void List<T>::push_back(const T& val) {
    Node<T>* node = new Node<T>(val, nullptr);
    if (!tail_) {
      head_ = tail_ = node_;
    } else {
      tail_->next = node;
      tail_ = node;
    }
    ++size;
  }
  template<class T>
  void List<T>::pop_front() {
    if (empty()) {
      return;
    }
    Node<T>* old = head_;
    head_ = head_->next;
    delete old;
    --size_;
  }
  template<class T>
  LIter<T> List<T>::insert_after(LIter<T> pos, const T& val) {
    if (!pos.ptr_) {
      return end();
    }
    Node<T>* node = new Node<T>(val, pos.ptr_->next);
    pos.ptr_->next = node;
    if (pos.ptr_ == tail_) {
      tail_ = node;
    }
    ++size_;
    return LIter<T>(node);
  }
  template<class T>
  LIter<T> List<T>::insert_after(LIter<T> pos, T&& val) {
    if (!pos.ptr_) {
      return end();
    }
    Node<T>* node = new Node<T>(std::move(val), pos.ptr_->next);
    pos.ptr_->next = node;
    if (pos.ptr_ == tail_) {
    tail_ = node;
    }
    ++size_;
    return LIter<T>(node);
  }
  template<class T>
  void List<T>::clear() noexcept {
    Node<T>* cur = head_;
    while (cur) {
      Node<T>* n = cur->next;
      delete cur;
      cur = n;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }
  template<class T>
  bool List<T>::empty() const noexcept {
    return head_ == nullptr;
  }
  template<class T>
  size_t List<T>::size() const noexcept {
    return size_;
  }
}
#endif
