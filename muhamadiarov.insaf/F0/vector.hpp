#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>

namespace muhamadiarov
{
  template < class T >
  class Vector
  {
  public:
    Vector();
    Vector(const Vector& other);
    explicit Vector(size_t capacity);
    Vector(Vector&& other) noexcept;

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;

    ~Vector();

    void popBack();
    void pushBack(const T& value);
    void pushBack(T&& value);
    void popFront();
    void pushFront(const T& value);
    void pushFront(T&& value);
    void insert(size_t id, const T& value);
    void insert(size_t id, T&& value);

    T& at(size_t id);
    const T& at(size_t id) const;

    void reserve(size_t newCapacity);
    void erase(size_t id) noexcept;
    void clear() noexcept;

    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
  private:
    T* data_;
    size_t size_;
    size_t capacity_;
    void shiftLeft(size_t id);
    void shiftRight(size_t id);
  };
}

namespace muh = muhamadiarov;

template < class T >
muh::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template < class T >
muh::Vector< T >::Vector(const Vector& other):
  data_(new T[other.capacity_]),
  size_(other.size_),
  capacity_(other.capacity_)
{
  for (size_t i = 0; i < other.size_; ++i)
  {
    data_[i] = other[i];
  }
}

template < class T >
muh::Vector< T >::Vector(size_t capacity):
  data_(new T[capacity]),
  size_(0),
  capacity_(capacity)
{}

template < class T >
muh::Vector< T >::Vector(Vector&& other) noexcept:
  data_(other.data_),
  size_(other.size_),
  capacity_(other.capacity_)
{
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template < class T >
muh::Vector< T >& muh::Vector< T >::operator=(const Vector& other)
{
  if (this != &other)
  {
    Vector< T > copy(other);
    std::swap(data_, copy.data_);
    std::swap(size_, copy.size_);
    std::swap(capacity_, copy.capacity_);
  }
  return *this;
}

template < class T >
muh::Vector< T >& muh::Vector< T >::operator=(Vector&& other) noexcept
{
  if (this != &other)
  {
    clear();
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

template < class T >
T& muh::Vector< T >::operator[](size_t id) noexcept
{
  return data_[id];
}

template < class T >
const T& muh::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template < class T >
muh::Vector< T >::~Vector()
{
  delete[] data_;
}

template < class T >
void muh::Vector< T >::popBack()
{
  if (size_ == 0)
  {
    return;
  }
  --size_;
  data_[size_] = T{};
}

template < class T >
void muh::Vector< T >::pushBack(const T& value)
{
  if (size_ >= capacity_)
  {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  data_[size_] = value;
  ++size_;
}

template < class T >
void muh::Vector< T >::pushBack(T&& value)
{
  if (size_ >= capacity_)
  {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  data_[size_] = std::move(value);
  ++size_;
}

template < class T >
void muh::Vector< T >::popFront()
{
  if (size_ == 0)
  {
    return;
  }
  shiftLeft(0);
}

template < class T >
void muh::Vector< T >::pushFront(const T& value)
{
  shiftRight(0);
  data_[0] = value;
}

template < class T >
void muh::Vector< T >::pushFront(T&& value)
{
  shiftRight(0);
  data_[0] = std::move(value);
}

template < class T >
void muh::Vector< T >::insert(size_t id, const T& value)
{
  if (id > size_)
  {
    throw std::out_of_range("Vector::insert: index out of range");
  }
  shiftRight(id);
  data_[id] = value;
}

template < class T >
void muh::Vector< T >::insert(size_t id, T&& value)
{
  if (id > size_)
  {
    throw std::out_of_range("Vector::insert: index out of range");
  }
  shiftRight(id);
  data_[id] = std::move(value);
}

template < class T >
void muh::Vector< T >::shiftLeft(size_t id)
{
  if (id >= size_)
  {
    return;
  }
  for (size_t i = id; i < size_ - 1; ++i)
  {
    data_[i] = std::move(data_[i + 1]);
  }
  --size_;
}

template < class T >
void muh::Vector< T >::shiftRight(size_t id)
{
  if (id > size_)
  {
    return;
  }
  if (size_ >= capacity_)
  {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  for (size_t i = size_; i > id; --i)
  {
    data_[i] = std::move(data_[i - 1]);
  }

  ++size_;
}

template < class T >
T& muh::Vector< T >::at(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("Vector::at: index out of range");
  }
  return data_[id];
}

template < class T >
const T& muh::Vector< T >::at(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("Vector::at: index out of range");
  }
  return data_[id];
}

template < class T >
void muh::Vector< T >::reserve(size_t newCapacity)
{
  if (newCapacity <= capacity_)
  {
    return;
  }
  T* newData = new T[newCapacity];
  for (size_t i = 0; i < size_; ++i)
  {
    newData[i] = std::move(data_[i]);
  }
  delete [] data_;
  data_ = newData;
  capacity_ = newCapacity;
}

template < class T >
void muh::Vector< T >::erase(size_t id) noexcept
{
  if (id >= size_)
  {
    return;
  }
  shiftLeft(id);
}

template < class T >
void muh::Vector< T >::clear() noexcept
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = T{};
  }
  size_ = 0;
}

template < class T >
bool muh::Vector< T >::isEmpty() const noexcept
{
  return size_ == 0;
}

template < class T >
size_t muh::Vector< T >::size() const noexcept
{
  return size_;
}

template < class T >
size_t muh::Vector< T >::capacity() const noexcept
{
  return capacity_;
}
#endif
