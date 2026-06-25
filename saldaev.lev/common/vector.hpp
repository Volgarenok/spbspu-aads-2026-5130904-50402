#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace saldaev
{
  template< class T >
  class Vector
  {
  public:
    class Iterator
    {
      friend class Vector;

    public:
      T &operator*() const;
      T *operator->() const;

      Iterator &operator++();
      Iterator operator++(int);
      Iterator &operator--();
      Iterator operator--(int);

      Iterator operator+(long long n) const;
      Iterator operator-(long long n) const;
      Iterator &operator+=(long long n);
      Iterator &operator-=(long long n);
      T &operator[](long long n) const;
      long long operator-(const Iterator &other) const;

      bool operator==(const Iterator &other) const;
      bool operator!=(const Iterator &other) const;
      bool operator<(const Iterator &other) const;
      bool operator<=(const Iterator &other) const;
      bool operator>(const Iterator &other) const;
      bool operator>=(const Iterator &other) const;

    private:
      T *curr_;

      Iterator(T *curr);
    };

    class ConstIterator
    {
      friend class Vector;

    public:
      const T &operator*() const;
      const T *operator->() const;

      ConstIterator &operator++();
      ConstIterator operator++(int);
      ConstIterator &operator--();
      ConstIterator operator--(int);

      ConstIterator operator+(long long n) const;
      ConstIterator operator-(long long n) const;
      ConstIterator &operator+=(long long n);
      ConstIterator &operator-=(long long n);
      const T &operator[](long long n) const;
      long long operator-(const ConstIterator &other) const;

      bool operator==(const ConstIterator &other) const;
      bool operator!=(const ConstIterator &other) const;
      bool operator<(const ConstIterator &other) const;
      bool operator<=(const ConstIterator &other) const;
      bool operator>(const ConstIterator &other) const;
      bool operator>=(const ConstIterator &other) const;

    private:
      const T *curr_;

      ConstIterator(T *curr);
    };

    // --- life cycle ---
    explicit Vector(size_t k);
    explicit Vector(std::initializer_list< T > il);
    Vector(const Vector< T > &);
    Vector(Vector< T > &&) noexcept;
    ~Vector();

    void swap(Vector< T > &rhs) noexcept;

    // --- assignment ---
    Vector< T > &operator=(const Vector< T > &);
    Vector< T > &operator=(Vector< T > &&) noexcept;

    // --- capacity & state ---
    bool isEmpty() const noexcept;
    size_t getCapacity() const noexcept;
    size_t getSize() const noexcept;
    void reserve(size_t cap);
    void shrinkToFit();

    // --- single element modifiers ---
    void pushBack(const T &);
    void pushFront(const T &);
    void popBack() noexcept;
    void remove(const T &);

    // --- range modifiers ---
    size_t pushBackRange(Iterator begin, size_t k);
    void insert(size_t i, const T &val);
    void erase(size_t i);
    void insert(size_t i, const Vector< T > &rhs, size_t beg, size_t end);
    void insert(size_t i, const Vector< T > &rhs);
    void erase(size_t beg, size_t end);
    void clear();

    // --- element access ---
    T &operator[](size_t index) noexcept;
    const T &operator[](size_t index) const noexcept;
    T &at(size_t id);
    const T &at(size_t id) const;

    // --- iterator ---
    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    void insert(Iterator here, const T &val);
    void insert(Iterator here, Iterator beg, Iterator end);
    void insert(Iterator here, std::initializer_list< T > list);
    void erase(Iterator beg, Iterator end);
    void erase(Iterator beg, size_t k);

  private:
    // --- data members ---
    T *data_;
    size_t size_, capacity_;

    // --- internal helpers ---
    void pushBackImpl(const T &val);
    void reserve(size_t pos, size_t count);
  };
}

// --- life cycle ---
template< class T >
saldaev::Vector< T >::Vector(size_t k):
  data_(static_cast< T * >(::operator new(sizeof(T) * k))),
  size_(0),
  capacity_(k)
{}

template< class T >
saldaev::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  size_ = 0;
  try {
    for (auto &&v : il) {
      new (data_ + size_) T(v);
      size_++;
    }
  } catch (...) {
    for (size_t j = 0; j < size_; ++j) {
      data_[j].~T();
    }
    ::operator delete(data_);
    throw;
  }
}

template< class T >
saldaev::Vector< T >::Vector(const Vector< T > &rhs):
  Vector(rhs.getSize())
{
  size_t i = 0;
  try {
    for (; i < rhs.getSize(); ++i) {
      new (data_ + i) T(rhs[i]);
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[j].~T();
    }
    ::operator delete(data_);
    throw;
  }
  size_ = rhs.getSize();
}

template< class T >
saldaev::Vector< T >::Vector(Vector &&rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template< class T >
saldaev::Vector< T >::~Vector()
{
  for (size_t i = 0; i < size_; ++i) {
    data_[i].~T();
  }

  ::operator delete(data_);
}

template< class T >
void saldaev::Vector< T >::swap(Vector< T > &rhs) noexcept
{
  std::swap(rhs.data_, data_);
  std::swap(rhs.size_, size_);
  std::swap(rhs.capacity_, capacity_);
}

// --- assignment ---
template< class T >
saldaev::Vector< T > &saldaev::Vector< T >::operator=(const Vector< T > &rhs)
{
  if (this != std::addressof(rhs)) {
    Vector< T > cpy(rhs);
    swap(cpy);
  }
  return *this;
}

template< class T >
saldaev::Vector< T > &saldaev::Vector< T >::operator=(Vector< T > &&rhs) noexcept
{
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

// --- capacity & state ---
template< class T >
bool saldaev::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
size_t saldaev::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
size_t saldaev::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void saldaev::Vector< T >::reserve(size_t cap)
{
  if (capacity_ >= cap) {
    return;
  }

  size_t i = 0;
  T *d = nullptr;
  try {
    d = static_cast< T * >(::operator new(sizeof(T) * cap));
    for (; i < getSize(); ++i) {
      new (d + i) T(std::move(data_[i]));
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      d[j].~T();
    }
    ::operator delete(d);
    throw;
  }

  for (size_t j = 0; j < size_; ++j)
    data_[j].~T();
  ::operator delete(data_);
  data_ = d;
  capacity_ = cap;
}

template< class T >
void saldaev::Vector< T >::shrinkToFit()
{
  if (capacity_ == size_) {
    return;
  }

  size_t i = 0;
  T *d = nullptr;
  try {
    d = static_cast< T * >(::operator new(sizeof(T) * size_));
    for (; i < getSize(); ++i) {
      new (d + i) T(std::move(data_[i]));
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      d[j].~T();
    }
    ::operator delete(d);
    throw;
  }

  for (size_t i = 0; i < size_; ++i)
    data_[i].~T();
  ::operator delete(data_);
  data_ = d;
  capacity_ = size_;
}

// --- single element modifiers ---
template< class T >
void saldaev::Vector< T >::pushBack(const T &val)
{
  if (size_ == capacity_) {
    reserve((capacity_ == 0) ? 1 : capacity_ * 2);
  }
  new (data_ + size_) T(val);
  ++size_;
}

template< class T >
void saldaev::Vector< T >::pushFront(const T &val)
{
  if (size_ == capacity_) {
    reserve((capacity_ == 0) ? 1 : capacity_ * 2);
  }

  for (size_t i = size_; i > 0; --i) {
    new (data_ + i) T(std::move(data_[i - 1]));
    data_[i - 1].~T();
  }
  new (data_) T(val);
  ++size_;
}

template< class T >
void saldaev::Vector< T >::popBack() noexcept
{
  if (size_) {
    --size_;
    data_[size_].~T();
  }
}

template< class T >
void saldaev::Vector< T >::remove(const T &d)
{
  for (size_t i = 0; i < size_; ++i) {
    if (data_[i] == d) {
      erase(i);
      return;
    }
  }
  throw std::out_of_range("Element not found in Vector");
}

// --- range modifiers ---
template< class T >
size_t saldaev::Vector< T >::pushBackRange(typename Vector< T >::Iterator begin, size_t k)
{
  reserve(size_ + k);
  size_t i = 0;
  try {
    for (; i < k; ++i, ++begin) {
      new (data_ + size_ + i) T(*begin);
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      data_[size_ + j].~T();
    }
    throw;
  }
  size_ += k;
  return k;
}

template< class T >
void saldaev::Vector< T >::insert(size_t i, const T &val)
{
  if (i > size_) {
    throw std::logic_error("");
  }
  Vector< T > cpy = *this;
  cpy.reserve(i, 1);
  new (cpy.data_ + i) T(val);
  ++cpy.size_;
  swap(cpy);
}

template< class T >
void saldaev::Vector< T >::erase(size_t i)
{
  if (i >= size_) {
    throw std::logic_error("");
  }
  Vector< T > cpy = *this;
  for (size_t j = i + 1; j < size_; ++j) {
    cpy.data_[j - 1] = std::move(cpy[j]);
  }
  --cpy.size_;
  cpy.data_[cpy.size_].~T();
  swap(cpy);
}

template< class T >
void saldaev::Vector< T >::insert(size_t i, const Vector< T > &rhs, size_t beg, size_t end)
{
  if (i > size_ || beg > end || rhs.getSize() < end) {
    throw std::logic_error("");
  }
  Vector< T > cpy = *this;
  size_t len = end - beg;
  cpy.reserve(i, len);
  for (size_t j = 0; j < len; ++j) {
    new (cpy.data_ + i + j) T(rhs[beg + j]);
  }
  cpy.size_ += len;
  swap(cpy);
}

template< class T >
void saldaev::Vector< T >::insert(size_t i, const Vector< T > &rhs)
{
  insert(i, rhs, 0, rhs.getSize());
}

template< class T >
void saldaev::Vector< T >::erase(size_t beg, size_t end)
{
  if (beg > end || getSize() < end) {
    throw std::logic_error("");
  }
  Vector< T > cpy = *this;
  size_t len = end - beg;
  for (size_t j = end; j < cpy.size_; ++j) {
    cpy.data_[j - len] = std::move(cpy[j]);
  }
  cpy.size_ -= len;
  for (size_t j = 0; j < len; ++j) {
    cpy.data_[cpy.size_ + j].~T();
  }
  swap(cpy);
}

template< class T >
void saldaev::Vector< T >::clear()
{
  erase(0, size_);
}

// --- element access ---
template< class T >
T &saldaev::Vector< T >::operator[](size_t index) noexcept
{
  assert(index < getSize());
  return data_[index];
}

template< class T >
const T &saldaev::Vector< T >::operator[](size_t index) const noexcept
{
  assert(index < getSize());
  return data_[index];
}

template< class T >
T &saldaev::Vector< T >::at(size_t id)
{
  const Vector< T > *cthis = this;
  const T &ret = cthis->at(id);
  return const_cast< T & >(ret);
}

template< class T >
const T &saldaev::Vector< T >::at(size_t id) const
{
  if (id < getSize()) {
    return data_[id];
  }
  throw std::range_error("bad id");
}

// --- iterator ---
template< class T >
typename saldaev::Vector< T >::Iterator saldaev::Vector< T >::begin() noexcept
{
  return Iterator(data_);
}

template< class T >
typename saldaev::Vector< T >::Iterator saldaev::Vector< T >::end() noexcept
{
  return Iterator(data_ + size_);
}

template< class T >
typename saldaev::Vector< T >::ConstIterator saldaev::Vector< T >::begin() const noexcept
{
  return ConstIterator(data_);
}

template< class T >
typename saldaev::Vector< T >::ConstIterator saldaev::Vector< T >::end() const noexcept
{
  return ConstIterator(data_ + size_);
}

template< class T >
void saldaev::Vector< T >::insert(Iterator here, const T &val)
{
  insert(here.curr_ - data_, val);
}

template< class T >
void saldaev::Vector< T >::insert(Iterator here, Iterator beg, Iterator end)
{
  if (beg > end) {
    throw std::logic_error("");
  }

  Vector< T > cpy = *this;
  size_t len = end - beg;
  size_t i = here - begin();
  cpy.reserve(i, len);
  for (size_t j = 0; j < len; ++j) {
    new (cpy.data_ + i + j) T(beg[j]);
  }
  cpy.size_ += len;
  swap(cpy);
}

template< class T >
void saldaev::Vector< T >::insert(Iterator here, std::initializer_list< T > list)
{
  Vector< T > cpy = *this;
  size_t i = here - begin();
  size_t len = list.size();
  cpy.reserve(i, len);
  for (size_t j = 0; j < len; ++j) {
    new (cpy.data_ + i + j) T(list[j]);
  }
  cpy.size_ += len;
  swap(cpy);
}

template< class T >
void saldaev::Vector< T >::erase(Iterator beg, Iterator end)
{
  if (beg > end) {
    throw std::logic_error("");
  }
  erase(beg - begin(), end - begin());
}

template< class T >
void saldaev::Vector< T >::erase(Iterator beg, size_t k)
{
  erase(beg - begin(), beg - begin() + k);
}

// --- internal helpers ---
template< class T >
void saldaev::Vector< T >::pushBackImpl(const T &val)
{
  new (data_ + size_) T(val);
  ++size_;
}

template< class T >
void saldaev::Vector< T >::reserve(size_t pos, size_t count)
{
  size_t i = 0;
  T *d = nullptr;
  try {
    d = static_cast< T * >(::operator new(sizeof(T) * (size_ + count)));
    for (; i < pos; ++i) {
      new (d + i) T(std::move(data_[i]));
    }
  } catch (...) {
    for (size_t j = 0; j < i; ++j) {
      d[j].~T();
    }
    ::operator delete(d);
    throw;
  }

  try {
    for (; i < size_; ++i) {
      new (d + count + i) T(std::move(data_[i]));
    }
  } catch (...) {
    size_t j = 0;
    for (; j < pos; ++j) {
      d[j].~T();
    }

    for (; j < i; ++j) {
      d[count + j].~T();
    }
    ::operator delete(d);
    throw;
  }

  for (size_t j = 0; j < size_; ++j)
    data_[j].~T();
  ::operator delete(data_);
  data_ = d;
  capacity_ = size_ + count;
}

// --- ITERATOR ---
template< class T >
saldaev::Vector< T >::Iterator::Iterator(T *curr):
  curr_(curr)
{}

template< class T >
T &saldaev::Vector< T >::Iterator::operator*() const
{
  return *curr_;
}

template< class T >
T *saldaev::Vector< T >::Iterator::operator->() const
{
  return curr_;
}

template< class T >
typename saldaev::Vector< T >::Iterator &saldaev::Vector< T >::Iterator::operator++()
{
  ++curr_;
  return *this;
}

template< class T >
typename saldaev::Vector< T >::Iterator saldaev::Vector< T >::Iterator::operator++(int)
{
  Iterator tmp = *this;
  ++curr_;
  return tmp;
}

template< class T >
typename saldaev::Vector< T >::Iterator &saldaev::Vector< T >::Iterator::operator--()
{
  --curr_;
  return *this;
}

template< class T >
typename saldaev::Vector< T >::Iterator saldaev::Vector< T >::Iterator::operator--(int)
{
  Iterator tmp = *this;
  --curr_;
  return tmp;
}

template< class T >
typename saldaev::Vector< T >::Iterator saldaev::Vector< T >::Iterator::operator+(long long n) const
{
  return Iterator(curr_ + n);
}

template< class T >
typename saldaev::Vector< T >::Iterator saldaev::Vector< T >::Iterator::operator-(long long n) const
{
  return Iterator(curr_ - n);
}

template< class T >
typename saldaev::Vector< T >::Iterator &saldaev::Vector< T >::Iterator::operator+=(long long n)
{
  curr_ += n;
  return *this;
}

template< class T >
typename saldaev::Vector< T >::Iterator &saldaev::Vector< T >::Iterator::operator-=(long long n)
{
  curr_ -= n;
  return *this;
}

template< class T >
T &saldaev::Vector< T >::Iterator::operator[](long long n) const
{
  return *(curr_ + n);
}

template< class T >
long long saldaev::Vector< T >::Iterator::operator-(const Vector< T >::Iterator &other) const
{
  return curr_ - other.curr_;
}

template< class T >
bool saldaev::Vector< T >::Iterator::operator==(const Iterator &other) const
{
  return curr_ == other.curr_;
}

template< class T >
bool saldaev::Vector< T >::Iterator::operator!=(const Iterator &other) const
{
  return !(*this == other);
}

template< class T >
bool saldaev::Vector< T >::Iterator::operator<(const Iterator &other) const
{
  return curr_ < other.curr_;
}

template< class T >
bool saldaev::Vector< T >::Iterator::operator<=(const Iterator &other) const
{
  return curr_ <= other.curr_;
}

template< class T >
bool saldaev::Vector< T >::Iterator::operator>(const Iterator &other) const
{
  return curr_ > other.curr_;
}

template< class T >
bool saldaev::Vector< T >::Iterator::operator>=(const Iterator &other) const
{
  return curr_ >= other.curr_;
}

// --- CONST ITERATOR ---

template< class T >
saldaev::Vector< T >::ConstIterator::ConstIterator(T *curr):
  curr_(curr)
{}

template< class T >
const T &saldaev::Vector< T >::ConstIterator::operator*() const
{
  return *curr_;
}

template< class T >
const T *saldaev::Vector< T >::ConstIterator::operator->() const
{
  return curr_;
}

template< class T >
typename saldaev::Vector< T >::ConstIterator &saldaev::Vector< T >::ConstIterator::operator++()
{
  ++curr_;
  return *this;
}

template< class T >
typename saldaev::Vector< T >::ConstIterator saldaev::Vector< T >::ConstIterator::operator++(int)
{
  ConstIterator tmp = *this;
  ++curr_;
  return tmp;
}

template< class T >
typename saldaev::Vector< T >::ConstIterator &saldaev::Vector< T >::ConstIterator::operator--()
{
  --curr_;
  return *this;
}

template< class T >
typename saldaev::Vector< T >::ConstIterator saldaev::Vector< T >::ConstIterator::operator--(int)
{
  ConstIterator tmp = *this;
  --curr_;
  return tmp;
}

template< class T >
typename saldaev::Vector< T >::ConstIterator saldaev::Vector< T >::ConstIterator::operator+(long long n) const
{
  return ConstIterator(curr_ + n);
}

template< class T >
typename saldaev::Vector< T >::ConstIterator saldaev::Vector< T >::ConstIterator::operator-(long long n) const
{
  return ConstIterator(curr_ - n);
}

template< class T >
typename saldaev::Vector< T >::ConstIterator &saldaev::Vector< T >::ConstIterator::operator+=(long long n)
{
  curr_ += n;
  return *this;
}

template< class T >
typename saldaev::Vector< T >::ConstIterator &saldaev::Vector< T >::ConstIterator::operator-=(long long n)
{
  curr_ -= n;
  return *this;
}

template< class T >
const T &saldaev::Vector< T >::ConstIterator::operator[](long long n) const
{
  return *(curr_ + n);
}

template< class T >
long long saldaev::Vector< T >::ConstIterator::operator-(const Vector< T >::ConstIterator &other) const
{
  return curr_ - other.curr_;
}

template< class T >
bool saldaev::Vector< T >::ConstIterator::operator==(const ConstIterator &other) const
{
  return curr_ == other.curr_;
}

template< class T >
bool saldaev::Vector< T >::ConstIterator::operator!=(const ConstIterator &other) const
{
  return !(*this == other);
}

template< class T >
bool saldaev::Vector< T >::ConstIterator::operator<(const ConstIterator &other) const
{
  return curr_ < other.curr_;
}

template< class T >
bool saldaev::Vector< T >::ConstIterator::operator<=(const ConstIterator &other) const
{
  return curr_ <= other.curr_;
}

template< class T >
bool saldaev::Vector< T >::ConstIterator::operator>(const ConstIterator &other) const
{
  return curr_ > other.curr_;
}

template< class T >
bool saldaev::Vector< T >::ConstIterator::operator>=(const ConstIterator &other) const
{
  return curr_ >= other.curr_;
}

#endif
