#ifndef MATVEEV_GAMEWORLD_ARRAY_HPP
#define MATVEEV_GAMEWORLD_ARRAY_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace matveev
{
template< class T >
class Array
{
public:
  Array():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  explicit Array(std::size_t count):
    data_(new T[count]()),
    size_(count),
    capacity_(count)
  {}

  Array(std::size_t count, const T& value):
    data_(new T[count]),
    size_(count),
    capacity_(count)
  {
    for (std::size_t i = 0; i < count; ++i)
    {
      data_[i] = value;
    }
  }

  Array(const Array& other):
    data_(other.capacity_ == 0 ? nullptr : new T[other.capacity_]),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    for (std::size_t i = 0; i < size_; ++i)
    {
      data_[i] = other.data_[i];
    }
  }

  Array(Array&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  Array& operator=(const Array& other)
  {
    if (this != &other)
    {
      Array temp(other);
      swap(temp);
    }

    return *this;
  }

  Array& operator=(Array&& other) noexcept
  {
    if (this != &other)
    {
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

  ~Array()
  {
    delete[] data_;
  }

  void swap(Array& other) noexcept
  {
    using std::swap;

    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
  }

  T& operator[](std::size_t index)
  {
    return data_[index];
  }

  const T& operator[](std::size_t index) const
  {
    return data_[index];
  }

  T& at(std::size_t index)
  {
    if (index >= size_)
    {
      throw std::out_of_range("array index out of range");
    }

    return data_[index];
  }

  const T& at(std::size_t index) const
  {
    if (index >= size_)
    {
      throw std::out_of_range("array index out of range");
    }

    return data_[index];
  }

  std::size_t size() const noexcept
  {
    return size_;
  }

  std::size_t capacity() const noexcept
  {
    return capacity_;
  }

  bool empty() const noexcept
  {
    return size_ == 0;
  }

  void pushBack(const T& value)
  {
    if (size_ == capacity_)
    {
      reserveExact(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    data_[size_] = value;
    ++size_;
  }

  void resize(std::size_t count, const T& value)
  {
    if (count > capacity_)
    {
      reserveExact(count);
    }

    for (std::size_t i = size_; i < count; ++i)
    {
      data_[i] = value;
    }

    size_ = count;
  }

  void clear() noexcept
  {
    size_ = 0;
  }

private:
  void reserveExact(std::size_t new_capacity)
  {
    T* new_data = new T[new_capacity];

    for (std::size_t i = 0; i < size_; ++i)
    {
      new_data[i] = std::move(data_[i]);
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }

  T* data_;
  std::size_t size_;
  std::size_t capacity_;
};
}

#endif
