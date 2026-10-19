#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <cassert>

namespace permyakov
{
  template< class T > class Vector
  {
    T * data;
    size_t size_, capasity_;
    explicit Vector(size_t k);
    public:
      Vector();
      Vector(const Vector< T > &);
      Vector(Vector< T > &&) noexcept;
      Vector< T > & operator=(const Vector< T > &);
      Vector< T > & operator=(Vector< T > &&) noexcept;
      ~Vector();
      explicit Vector(std::initializer_list< T > il);

      T & operator[](size_t id) noexcept;
      const T & operator[](size_t id) const noexcept;

      size_t size() const noexcept;
      size_t capasity() const noexcept;
      bool isEmpty() const noexcept;

      void reserve(size_t cap);
      void swap(Vector< T > &rhs) noexcept;

      void pushFront(const T & val);
      void popFront();
      void pushBack(const T & val);
      void popBack();

      void insert(size_t idx, const T & val);

      void erase(size_t idx);
      void erase(size_t beg, size_t end);

      void clear(T * beg, size_t size);
  };

  template< class T > Vector< T >::Vector():
    data(nullptr),
    size_(0),
    capasity_(0)
  {}

  template< class T > Vector< T >::Vector(size_t k):
    data(static_cast< T * >(::operator new(sizeof(T) * k))),
    size_(0),
    capasity_(k)
  {}

  template< class T > Vector< T >::Vector(const Vector< T > & scnd):
    Vector(scnd.size())
  {
    while (size_ < scnd.size()) {
      new (data + size_) T(scnd[size_]);
      ++size_;
    }
  }

  template< class T > Vector< T >::Vector(Vector< T > && scnd) noexcept:
    data(scnd.data),
    size_(scnd.size_),
    capasity_(scnd.capasity_)
  {
    scnd.data = nullptr;
    scnd.size_ = 0;
    scnd.capasity_ = 0;
  }

  template< class T > Vector< T > & Vector< T >::operator=(const Vector< T > & scnd)
  {
    if (this == std::addressof(scnd)) {
      return *this;
    }
    Vector< T > copy(scnd);
    swap(copy);
    return *this;
  }

  template< class T > Vector< T > & Vector< T >::operator=(Vector< T > && scnd) noexcept
  {
    if (this == std::addressof(scnd)) {
      return *this;
    }
    Vector< T > copy(std::move(scnd));
    swap(copy);
    return *this;
  }

  template< class T > Vector< T >::~Vector()
  {
    clear(data, size_);
    ::operator delete(data);
  }

  template< class T > Vector< T >::Vector(std::initializer_list< T > il):
    Vector(il.size())
  {
    for (auto && v : il) {
      new (data + size_++) T(std::move(v));
    } 
  }

  template< class T > T & Vector< T >::operator[](size_t id) noexcept
  {
    assert(id < size());
    return data[id];
  }

  template< class T > const T & Vector< T >::operator[](size_t id) const noexcept
  {
    assert(id < size());
    return data[id];
  }

  template< class T > size_t Vector< T >::size() const noexcept
  {
    return size_;
  }

  template< class T > size_t Vector< T >::capasity() const noexcept
  {
    return capasity_;
  }

  template< class T > bool Vector< T >::isEmpty() const noexcept
  {
    return !size_;
  }

  template< class T > void Vector< T >::reserve(size_t cap)
  {
    if (capasity_ >= cap) {
      return;
    }
    size_t k = 0;
    T * newData = static_cast< T * >(::operator new(sizeof(T) * cap));
    try {
      while (k < size()) {
        new (newData + k) T(std::move(data[k]));
        ++k;
      }
    } catch (...) {
      clear(newData, k);
      throw;
    }
    clear(data, size_);
    data = newData;
    capasity_ = cap;
  }

  template< class T > void Vector< T >::swap(Vector< T > & scnd) noexcept
  {
    std::swap(scnd.data, data);
    std::swap(scnd.capasity_, capasity_);
    std::swap(scnd.size_, size_);
  }

  template< class T > void Vector< T >::pushFront(const T & val)
  {
    insert(0, val);
  }

  template< class T > void Vector< T >::popFront()
  {
    erase(0);
  }

  template< class T > void Vector< T >::pushBack(const T & val)
  {
    if (size_ == capasity_) {
      size_t newCap = capasity_ == 0 ? 1 : capasity_ * 2;
      reserve(newCap);
    }
    new (data + size_++) T(val);
  }

  template< class T > void Vector< T >::popBack()
  {
    if (isEmpty()) {
      throw std::logic_error("Empty vector");
    }
    data[size_ - 1].~T();
    --size_;
  }

  template< class T > void Vector< T >::insert(size_t idx, const T & val)
  {
    if (size_ == capasity_) {
      size_t newCap = capasity_ == 0 ? 1 : capasity_ * 2;
      reserve(newCap);
    }
    ++size_;
    for (size_t i = size_ - 1; i > idx; --i) {
      new (data + i) T(std::move(data[i - 1]));
      data[i - 1].~T();
    }
    new (data + idx) T(val);
  }


  template< class T > void Vector< T >::erase(size_t idx)
  {
    if (isEmpty()) {
      throw std::logic_error("Empty vector");
    }
    data[idx].~T();
    for (size_t i = idx; i < size_ - 1; ++i) {
      new (data + i) T(std::move(data[i + 1]));
      data[i + 1].~T();
    }
    --size_;
  }

  template< class T > void Vector< T >::erase(size_t beg, size_t end)
  {
    if (isEmpty()) {
      throw std::logic_error("Empty vector");
    }
    if (end < beg) {
      throw std::logic_error("Wrong index");
    }
    size_t dif = end - beg;
    for (size_t i = beg; i + dif < size_; ++i) {
        data[i] = std::move(data[i + dif]);
    }
    for (size_t i = size_ - dif; i < size_; ++i) {
        data[i].~T();
    }
    size_ -= dif;
  }

  template< class T > void Vector< T >::clear(T * beg, size_t len) {
    for (size_t i = 0; i < len; ++i) {
      beg[i].~T();
    }
  }
}

#endif
