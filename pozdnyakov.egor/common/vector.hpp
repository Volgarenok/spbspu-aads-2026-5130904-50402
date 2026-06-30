#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <utility>

namespace pozdnyakov
{
  template< class T >
  class Vector
  {
  private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(std::size_t newCapacity)
    {
      if (newCapacity == 0) {
        newCapacity = 1;
      }
      T *newData = new T[newCapacity];
      for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = std::move(data_[i]);
      }
      delete[] data_;
      data_ = newData;
      capacity_ = newCapacity;
    }

  public:
    using value_type = T;
    using iterator = T *;
    using const_iterator = const T *;
    using reference = T &;
    using const_reference = const T &;

    Vector():
      data_(nullptr),
      size_(0),
      capacity_(0)
    {}

    Vector(std::size_t count, const T &value = T()):
      data_(nullptr),
      size_(0),
      capacity_(0)
    {
      if (count > 0) {
        reallocate(count);
        for (std::size_t i = 0; i < count; ++i) {
          data_[i] = value;
        }
        size_ = count;
      }
    }

    Vector(const Vector &other):
      data_(nullptr),
      size_(0),
      capacity_(0)
    {
      if (other.capacity_ > 0) {
        reallocate(other.capacity_);
        for (std::size_t i = 0; i < other.size_; ++i) {
          data_[i] = other.data_[i];
        }
        size_ = other.size_;
      }
    }

    Vector(Vector &&other) noexcept:
      data_(other.data_),
      size_(other.size_),
      capacity_(other.capacity_)
    {
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }

    Vector &operator=(Vector other)
    {
      std::swap(data_, other.data_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      return *this;
    }

    ~Vector()
    {
      delete[] data_;
    }

    void pushBack(const T &value)
    {
      if (size_ == capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
      }
      data_[size_++] = value;
    }

    void pushBack(T &&value)
    {
      if (size_ == capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
      }
      data_[size_++] = std::move(value);
    }

    reference operator[](std::size_t index)
    {
      return data_[index];
    }

    const_reference operator[](std::size_t index) const
    {
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

    void clear() noexcept
    {
      size_ = 0;
    }

    void resize(std::size_t newSize)
    {
      if (newSize > capacity_) {
        reallocate(newSize);
      }
      if (newSize > size_) {
        for (std::size_t i = size_; i < newSize; ++i) {
          data_[i] = T();
        }
      }
      size_ = newSize;
    }

    iterator begin() noexcept
    {
      return data_;
    }

    iterator end() noexcept
    {
      return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
      return data_;
    }

    const_iterator end() const noexcept
    {
      return data_ + size_;
    }

    const_iterator cbegin() const noexcept
    {
      return data_;
    }

    const_iterator cend() const noexcept
    {
      return data_ + size_;
    }
  };
}

#endif
