#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <cstddef>
#include <utility>

namespace alisov
{
  template < class T >
  class Vector
  {
  private:
    T *data_;
    size_t size_;
    size_t capacity_;

  public:
    Vector():
      data_(nullptr),
      size_(0),
      capacity_(0)
    {}
    ~Vector()
    {
      delete[] reinterpret_cast< char * >(data_);
    }
    void reserve(size_t new_cap)
    {
      if (new_cap <= capacity_)
        return;
      T *new_data = reinterpret_cast< T * >(new char[new_cap * sizeof(T)]);
      for (size_t i = 0; i < size_; ++i) {
        new (&new_data[i]) T(std::move(data_[i]));
        data_[i].~T();
      }
      delete[] reinterpret_cast< char * >(data_);
      data_ = new_data;
      capacity_ = new_cap;
    }
    void push_back(const T &value)
    {
      if (size_ >= capacity_) {
        reserve(capacity_ == 0 ? 4 : capacity_ * 2);
      }
      new (&data_[size_]) T(value);
      ++size_;
    }
    size_t size() const
    {
      return size_;
    }
    bool empty() const
    {
      return size_ == 0;
    }
    T &operator[](size_t index)
    {
      return data_[index];
    }
    const T &operator[](size_t index) const
    {
      return data_[index];
    }

    void erase(size_t index)
    {
      data_[index].~T();
      for (size_t i = index; i < size_ - 1; ++i) {
        new (&data_[i]) T(std::move(data_[i + 1]));
        data_[i + 1].~T();
      }
      --size_;
    }
    Vector(const Vector &other):
      data_(nullptr),
      size_(0),
      capacity_(0)
    {
      reserve(other.size_);
      for (size_t i = 0; i < other.size_; ++i) {
        new (&data_[i]) T(other.data_[i]);
      }
      size_ = other.size_;
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
    void swap(Vector &other) noexcept
    {
      std::swap(data_, other.data_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
    }
  };
}
#endif
