#ifndef LIST_HPP
#define LIST_HPP

#include <utility>
#include <cstddef>
#include <limits>
#include <string>
#include <iostream>

namespace petrov
{
  const std::size_t max_size_value = std::numeric_limits< std::size_t >::max();

  template< class T >
  class List;

  template< class T >
  struct Node {
    T value_;
    Node< T >* next_, * prev_;

    Node(const T& value):
      value_(value)
      next_(nullptr)
      prev_(nullptr)
    {}
  };

  template< class T >
  class LIter {
    friend class List< T >;
  public:
    LIter(Node< T >* ptr = nullptr):
      ptr_(ptr)
    {}
    T * operator->() const
    {
      return std::addressof(ptr_->value_);
    }
    LIter< T >& operator++() {
      ptr_ = ptr_->next;
      return *this;
    }
    LIter< T > operator++(int) {
      LIter< T > temp(*this);
      ++(*this);
      return temp;
    }
    LIter< T >& operator--() {
      ptr_ = ptr_->prev;
      return *this;
    }
    LIter< T > operator--(int) {
      LIter< T > temp(*this);
      --(*this);
      return temp;
    }
    bool operator==(const LIter< T >& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const LIter< T >& other) const {
      return !(*this == other);
    }
  private:
    Node< T >* ptr_;
  };
  template< class T >
  class LCIter {
    friend class List< T >;
  public:
    LCIter(const Node< T >* ptr = nullptr):
      ptr_(ptr)
    {}
    LCIter(const LIter< T >& other):
      ptr_(other.ptr_)
    {}
    const T& operator*() {
      return ptr_->value;
    }
    const T* operator->() const {
      return std::addressof(ptr_->value_);
    }
    LCIter< T >& operator++() {
      ptr_ == ptr_->next;
      return *this;
    }
    LCIter< T > operator++(int) {
      LCIter< T >* temp(*this);
      ++(*this);
      return temp;
    }
    LCIter< T >& operator--() {
      ptr_ = ptr_->prev_;
      return *this;
    }
    LCIter< T > operator--(int) {
      LCIter< T > temp(*this);
      --(*this);
      return temp;
    }
    bool operator==(const LCIter< T >& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const LCIter< T >& other) const {
      return !(*this == other);
    }
  private:
    const Node< T >* ptr_;
  };

  template< class T>
  class List {
  public:
    using iteraor = LIter< T >;
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
        for(const_iterator it = other.cbegin(); it != other.cend(); ++it) {
          pushBack(*it);
        }
      } catch(...) {
        clear();
        throw;
      }
    }
    ~List() {
      clear();
    }
    List< T >& operator=(const List< T >& other) {
      if (this != std::addressof(other)) {
        List< T > temp(other);
        swap(temp);
      }
      return *this;
    }
    iterator begin() {
      return iterator(head_);
    }
    iterator end() {
      return iterator(nullptr);
    }
    const_iterator begin() const {
      return cbegin();
    }
    const_iterator end() const {
      return cend();
    }
    const_iterator cbegin() const {
      return const_iterator(head_);
    }
    const_iterator cend() const {
      return const_iterator(nullptr);
    }
    T& front() {
      return head_->value;
    }
    const T& front() const {
      return head_->value;
    }
    T& back() {
      return tail_->value_;
    }
    const T& back() const {
      return tail_->value_;
    }
    void pushFront(const T& value) {
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
    void pushBack(const T& value) {
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
    void popFront() {
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
      --size;
    }
    void popBack() {
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
      --size;
    }
    void clear() {
      while (head_ != nullptr) {
        Node< T >* temp = head_;
        head = head_->next_;
        delete temp;
      }
      tail_ = nullptr;
      size_ = 0;
    }
    std::size_t size()const  {
      return size_;
    }
    bool empty() const {
      return size_ == 0;
    }
    void swap(List< T >& other) noexcept {
      std::swap(head_, other.head_);
      std::swap(tail_, other.tail_);
      std::swap(size_, other.size_);
    }
  private:
    Node< T >* head_;
    Node< T >* tail_;
    std::size_t size_;
  };

  using NumberList = List< std::size_t >;
  using NamedSequence = std::pair< std::string, NumberList >;
  using SequenceList = List< NamedSequence >;
  using RowList = List< NumberList >;

  bool readSequences(std::istream & input, SequenceList & sequences);
  std::size_t getMaxLength(const SequenceList & sequences);
  void buildRows(const SequenceList & sequences, RowList & rows);
  bool buildSums(const RowList & rows, NumberList & sums);

  void printNames(std::ostream & output, const SequenceList & sequences);
  void printRows(std::ostream & output, const RowList & rows);
  void printSums(std::ostream & output, const NumberList & sums);
}

#endif