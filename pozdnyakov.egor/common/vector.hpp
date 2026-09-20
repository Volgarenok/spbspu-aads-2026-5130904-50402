#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <utility>

namespace pozdnyakov
{
  template< class T >
  class Vector
  {
  public:
    using value_type = T;
    using iterator = T *;
    using const_iterator = const T *;
    using reference = T &;
    using const_reference = const T &;

    Vector() noexcept:
      data_(nullptr),
      size_(0),
      capacity_(0)
    {}

    explicit Vector(std::size_t count, const T &value = T()):
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
      if (other.size_ > 0) {
        reallocate(other.size_);
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

    ~Vector()
    {
      delete[] data_;
    }

    Vector &operator=(Vector other) noexcept
    {
      swap(other);
      return *this;
    }

    reference operator[](std::size_t index)
    {
      return data_[index];
    }

    const_reference operator[](std::size_t index) const
    {
      return data_[index];
    }

    void swap(Vector &other) noexcept
    {
      std::swap(data_, other.data_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
    }

    void pushBack(const T &value)
    {
      if (size_ == capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
      }
      data_[size_] = value;
      ++size_;
    }

    void pushBack(T &&value)
    {
      if (size_ == capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
      }
      data_[size_] = std::move(value);
      ++size_;
    }

    void popBack()
    {
      if (size_ > 0) {
        --size_;
        data_[size_] = T();
      }
    }

    void resize(std::size_t newSize)
    {
      if (newSize > capacity_) {
        reallocate(newSize);
      } else {
        for (std::size_t i = size_; i < newSize; ++i) {
          data_[i] = T();
        }
      }
      for (std::size_t i = newSize; i < size_; ++i) {
        data_[i] = T();
      }
      size_ = newSize;
    }

    void clear()
    {
      resize(0);
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

    reference front()
    {
      return data_[0];
    }

    const_reference front() const
    {
      return data_[0];
    }

    reference back()
    {
      return data_[size_ - 1];
    }

    const_reference back() const
    {
      return data_[size_ - 1];
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

  private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(std::size_t newCapacity)
    {
      if (newCapacity == 0) {
        newCapacity = 1;
      }
      T *newData = new T[newCapacity]();
      try {
        for (std::size_t i = 0; i < size_; ++i) {
          newData[i] = std::move(data_[i]);
        }
      } catch (...) {
        delete[] newData;
        throw;
      }
      delete[] data_;
      data_ = newData;
      capacity_ = newCapacity;
    }
  };
}

#endif
