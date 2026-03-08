#ifndef LIST_HPP
#define LIST_HPP
namespace sogdanov {
  template<class T>
  struct Node {
    T data;
    Node* next;
  };
  template<class T>
  class LIter {
    friend class List<T>;
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
    const Node<T>* ptr_;
  public:
    LCIter() noexcept;
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
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void push_front(const T& val);
    void push_front(T&& val);
    void pop_front();
    void pop_back();
    LIter<T> insert_after(LIter<T> pos, const T& val);
    LIter<T> insert_after(LIter<T> pos, T&& val);
    void clear() noexcept;
    bool empty() const noexcept;
    size_t size() const noexcept;
  };
  template<class T>
  LIter<T>::LIter() noexcept : ptr_(nullptr) {}
  template<class T>
  LIter<T>::LIter(Node<T>* p) noexcept : ptr_(p) {}
  template<class T>
  T& LIter<T>::operator*() const { return ptr_->data; }
  template<class T>
  LIter<T>& LIter<T>::operator++() noexcept { ptr_ = ptr_->next; return *this; }
  template<class T>
  bool LIter<T>::operator==(const LIter& o) const noexcept { return ptr_ == o.ptr_; }
  template<class T>
  bool LIter<T>::operator!=(const LIter& o) const noexcept { return ptr_ != o.ptr_; }
