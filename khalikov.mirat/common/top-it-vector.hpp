#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>
#include <utility>
#include <cassert>
#include <stdexcept>
#include <initializer_list>
#include <new>
#include "iter.hpp"
#include "citer.hpp"

namespace khalikov
{
  template < class T > struct Vector
  {
    Vector();
    Vector(size_t k);
    explicit Vector(std::initializer_list< T >);
    ~Vector();

    Vector(const Vector< T > &);
    Vector &operator=(const Vector< T > &rhs);
    T &operator[](size_t id) noexcept;
    const T &operator[](size_t id) const noexcept;
    Vector(Vector< T > &&) noexcept;
    Vector< T > &operator=(Vector< T > &&) noexcept;

    bool isEmpty() const noexcept;
    void pushBack(const T &);
    void popBack();
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void resize(size_t newCapacity);
    void swap(Vector< T > &rhs) noexcept;
    void pushFront(const T &);
    T &at(size_t id);
    const T &at(size_t id) const;
    void reserve(size_t cap);
    void shrinkToFit();

    template < class IT > size_t pushBackRange(IT begin, size_t k);

    void erase(size_t i);
    void erase(size_t beg, size_t end);
    void erase(LIter< T > pos);
    void erase(LIter< T > first, LIter< T > last);
    void erase(LIter< T > pos, size_t k);
    void insert(size_t i, const T &val);
    void insert(size_t i, const Vector< T > &rhs, size_t beg, size_t end);
    void insert(LIter< T > pos, const T &val);
    void insert(LIter< T > pos, const Vector< T > &other);
    void insert(LIter< T > pos, size_t k, const T &val);

  private:
    T *data_;
    size_t cap_, size_;
    friend class LIter< T >;
    friend class LCIter< T >;
    void pushBackImpl(const T &);
  };
}

template < class T > void khalikov::Vector< T >::erase(size_t i)
{
  assert(i < size_);
  for (size_t j = i; j < size_ - 1; ++j) {
    data_[j] = std::move(data_[j + 1]);
  }
  data_[size_ - 1].~T();
  --size_;
}

template < class T > void khalikov::Vector< T >::erase(size_t beg, size_t end)
{
  assert(beg <= end && end <= size_);
  if (beg == end)
    return;
  size_t k = end - beg;
  for (size_t j = beg; j < size_ - k; ++j) {
    data_[j] = std::move(data_[j + k]);
  }
  for (size_t j = size_ - k; j < size_; ++j) {
    data_[j].~T();
  }
  size_ -= k;
}

template < class T > void khalikov::Vector< T >::erase(LIter< T > first, LIter< T > last)
{
  size_t beg = &(*first) - data_;
  size_t end = &(*last) - data_;
  erase(beg, end);
}

template < class T > void khalikov::Vector< T >::erase(LIter< T > pos, size_t k)
{
  size_t beg = &(*pos) - data_;
  erase(beg, beg + k);
}

template < class T > void khalikov::Vector< T >::erase(LIter< T > pos)
{
  size_t index = &(*pos) - data_;
  erase(index);
}

template < class T > void khalikov::Vector< T >::insert(size_t i, const Vector< T > &rhs, size_t beg, size_t end)
{
  assert(i <= size_);
  assert(beg <= end && end <= rhs.size_);
  if (beg == end) {
    return;
  }
  size_t count = end - beg;
  if (size_ + count > cap_) {
    size_t newCap = cap_ == 0 ? count : cap_;
    while (newCap < size_ + count) {
      newCap *= 2;
    }
    reserve(newCap);
  }
  size_t moveCount = size_ - i;
  for (size_t j = 0; j < count; ++j) {
    if (size_ - j > i) {
      new (data_ + size_ + count - 1 - j) T(std::move(data_[size_ - 1 - j]));
    }
  }
  for (size_t j = size_ - 1; j >= i + count && j < size_; --j) {
    data_[j] = std::move(data_[j - count]);
  }
  for (size_t j = 0; j < count; ++j) {
    if (i + j < size_) {
      data_[i + j] = rhs.data_[beg + j];
    } else {
      new (data_ + i + j) T(rhs.data_[beg + j]);
    }
  }
  size_ += count;
}

template < class T > void khalikov::Vector< T >::insert(size_t i, const T &val)
{
  assert(i <= size_);
  if (i == size_) {
    pushBack(val);
    return;
  }
  if (size_ == cap_) {
    size_t newCapacity = cap_ == 0 ? 4 : cap_ * 2;
    reserve(newCapacity);
  }
  new (data_ + size_) T(std::move(data_[size_ - 1]));
  for (size_t j = size_ - 1; j > i; --j) {
    data_[j] = std::move(data_[j - 1]);
  }
  data_[i] = val;
  ++size_;
}

template < class T > void khalikov::Vector< T >::insert(LIter< T > pos, const T &val)
{
  size_t index = &(*pos) - data_;
  insert(index, val);
}

template < class T > void khalikov::Vector< T >::insert(LIter< T > pos, const Vector< T > &other)
{
  size_t index = &(*pos) - data_;
  insert(index, other, 0, other.getSize());
}

template < class T > void khalikov::Vector< T >::insert(LIter< T > pos, size_t k, const T &val)
{
  if (k == 0)
    return;
  size_t index = &(*pos) - data_;
  assert(index <= size_);
  Vector< T > temp;
  temp.reserve(k);
  for (size_t j = 0; j < k; ++j) {
    temp.pushBack(val);
  }
  insert(index, temp, 0, k);
}

template < class T > void khalikov::Vector< T >::reserve(size_t cap)
{
  if (cap_ >= cap) {
    return;
  }
  size_t i = 0;
  T *d = static_cast< T * >(::operator new(sizeof(T) * cap));
  try {
    for (; i < getSize(); i++) {
      new (d + i) T(std::move(data_[i]));
    }
  } catch (const std::bad_alloc &) {
    for (size_t j = 0; j < i; ++j) {
      (d + j)->~T();
    }
    ::operator delete(d);
    throw std::bad_alloc();
  }
  ::operator delete(data_);
  data_ = d;
  cap_ = cap;
}

template < class T > template < class IT > size_t khalikov::Vector< T >::pushBackRange(IT begin, size_t k)
{
  size_t oldSize = size_;
  size_t newSize = size_ + k;
  if (newSize > cap_) {
    size_t newCap = cap_ == 0 ? k : cap_;
    while (newCap < newSize) {
      newCap = newCap * 2;
    }
    reserve(newCap);
  }
  for (size_t i = 0; i < k; ++i) {
    new (data_ + size_) T(*begin);
    ++size_;
    ++begin;
  }
  return newSize;
}

template < class T > void khalikov::Vector< T >::pushBackImpl(const T &val)
{
  new (data_ + size_) T(val);
  ++size_;
}

template < class T > void khalikov::Vector< T >::shrinkToFit()
{
  if (size_ == cap_) {
    return;
  }
  Vector< T > temp(size_);
  for (size_t i = 0; i < size_; ++i) {
    new (temp.data_ + temp.size_) T(std::move(data_[i]));
    temp.size_++;
  }
  swap(temp);
}

template < class T >
khalikov::Vector< T >::Vector(std::initializer_list< T > il):
  Vector< T >(il.size())
{
  try {
    for (auto &&v : il) {
      new (data_ + size_) T(v);
      ++size_;
    }
  } catch (const std::bad_alloc &) {
    this->~Vector();
    throw std::bad_alloc();
  }
}

template < class T > size_t khalikov::Vector< T >::getSize() const noexcept
{
  return size_;
}

template < class T > void khalikov::Vector< T >::pushFront(const T &val)
{
  Vector< T > cpy(size_ + 1);
  new (cpy.data_ + cpy.size_) T(val);
  cpy.size_++;
  for (size_t i = 0; i < size_; ++i) {
    new (cpy.data_ + cpy.size_) T(std::move(data_[i]));
    cpy.size_++;
  }
  swap(cpy);
}

template < class T > size_t khalikov::Vector< T >::getCapacity() const noexcept
{
  return cap_;
}

template < class T > bool khalikov::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template < class T > void khalikov::Vector< T >::popBack()
{
  assert(size_ > 0);
  data_[size_ - 1].~T();
  --size_;
}

template < class T >
khalikov::Vector< T >::Vector():
  data_(nullptr),
  cap_(0),
  size_(0)
{}

template < class T >
khalikov::Vector< T >::Vector(size_t k):
  data_(static_cast< T * >(::operator new(sizeof(T) * k))),
  cap_(k),
  size_(0)
{}

template < class T > khalikov::Vector< T >::~Vector()
{
  if (data_) {
    for (size_t i = 0; i < size_; ++i) {
      data_[i].~T();
    }
    ::operator delete(data_);
  }
}

template < class T >
khalikov::Vector< T >::Vector(const Vector< T > &rhs):
  data_(nullptr),
  cap_(0),
  size_(0)
{
  Vector< T > temp(rhs.cap_);
  try {
    for (size_t i = 0; i < rhs.size_; i++) {
      new (temp.data_ + i) T(rhs.data_[i]);
      temp.size_++;
    }
  } catch (const std::bad_alloc &) {
    throw std::bad_alloc();
  }
  swap(temp);
}

template < class T >
khalikov::Vector< T >::Vector(Vector< T > &&rhs) noexcept:
  data_(rhs.data_),
  cap_(rhs.cap_),
  size_(rhs.size_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.cap_ = 0;
}

template < class T > khalikov::Vector< T > &khalikov::Vector< T >::operator=(Vector< T > &&rhs) noexcept
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template < class T > void khalikov::Vector< T >::swap(Vector< T > &rhs) noexcept
{
  std::swap(rhs.data_, data_);
  std::swap(rhs.size_, size_);
  std::swap(rhs.cap_, cap_);
}

template < class T > khalikov::Vector< T > &khalikov::Vector< T >::operator=(const Vector< T > &rhs)
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template < class T > void khalikov::Vector< T >::resize(size_t newCapacity)
{
  Vector< T > res(newCapacity);
  for (size_t i = 0; i < size_ && i < newCapacity; i++) {
    new (res.data_ + res.size_) T(std::move(data_[i]));
    res.size_++;
  }
  while (res.size_ < newCapacity) {
    new (res.data_ + res.size_) T();
    res.size_++;
  }
  swap(res);
}

template < class T > T &khalikov::Vector< T >::operator[](size_t id) noexcept
{
  assert(id < getSize());
  return data_[id];
}

template < class T > const T &khalikov::Vector< T >::operator[](size_t id) const noexcept
{
  assert(id < getSize());
  return data_[id];
}

template < class T > T &khalikov::Vector< T >::at(size_t id)
{
  if (id >= size_) {
    throw std::out_of_range("Vector index out of range");
  }
  return data_[id];
}

template < class T > const T &khalikov::Vector< T >::at(size_t id) const
{
  if (id >= size_) {
    throw std::out_of_range("Vector index out of range");
  }
  return data_[id];
}

template < class T > void khalikov::Vector< T >::pushBack(const T &val)
{
  if (cap_ > size_) {
    new (data_ + size_) T(val);
    size_++;
  } else {
    size_t newCapacity = cap_ == 0 ? 4 : cap_ * 2;
    reserve(newCapacity);
    new (data_ + size_) T(val);
    size_++;
  }
}

#endif
