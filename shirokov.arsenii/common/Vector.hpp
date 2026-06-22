#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <new>
#include <stdexcept>
#include <utility>

namespace shirokov
{
  template< class T >
  struct Vector;

  template< class T >
  struct VIter
  {
    explicit VIter(Vector< T >& v, size_t pos);
    bool operator==(const VIter< T >&) const noexcept;
    bool operator!=(const VIter< T >&) const noexcept;
    VIter< T >& operator++() noexcept;
    VIter< T >& operator--() noexcept;
    VIter< T > operator+(size_t i) noexcept;
    VIter< T > operator-(size_t i) noexcept;
    T& operator*();

  private:
    Vector< T >& v_;
    size_t pos_;
    friend class Vector< T >;
  };

  template< class T >
  struct VCIter
  {
    explicit VCIter(const Vector< T >& v, size_t pos);
    bool operator==(const VCIter< T >&) const noexcept;
    bool operator!=(const VCIter< T >&) const noexcept;
    VCIter< T >& operator++() noexcept;
    VCIter< T >& operator--() noexcept;
    VCIter< T > operator+(size_t i) noexcept;
    VCIter< T > operator-(size_t i) noexcept;
    const T& operator*();

  private:
    const Vector< T >& v_;
    size_t pos_;
    friend class Vector< T >;
  };

  template< class T >
  struct Vector
  {
    Vector();
    Vector(const Vector< T >&);
    Vector(Vector< T >&&) noexcept;
    explicit Vector(std::initializer_list< T > il);

    ~Vector();
    Vector< T >& operator=(const Vector< T >&);
    Vector< T >& operator=(Vector< T >&&) noexcept;

    VIter< T > begin();
    VCIter< T > begin() const;
    VCIter< T > cbegin() const;

    VIter< T > end();
    VCIter< T > end() const;
    VCIter< T > cend() const;

    void swap(Vector< T >& rhs) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void reserve(size_t cap);
    void shrinkToFit();

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    void pushBack(const T&);
    void popBack();
    void pushFront(const T&);
    void popFront();

    void insert(size_t i, const T& val);
    void erase(size_t i);
    void insert(size_t i, const Vector< T >& rhs, size_t beg, size_t end);
    void erase(size_t beg, size_t end);

    template< class FwdIterator >
    void insert(VIter< T > pos, FwdIterator beg, FwdIterator end);
    void insert(VIter< T > pos, const T& value);
    void insert(const T& value, size_t count, VIter< T > pos);
    void erase(VIter< T > beg, VIter< T > end);
    void erase(VIter< T > pos);
    void erase(VIter< T > pos, size_t count);

  private:
    T* data_;
    size_t size_, capacity_;

    explicit Vector(size_t k);
    void pushBackImpl(const T&);
    void reserve(size_t pos, size_t k);
  };

  template< class T >
  void clear(T* data, size_t to_pos);
}

template< class T >
void shirokov::Vector< T >::shrinkToFit()
{
  Vector< T > cpy(*this);
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::reserve(size_t pos, size_t k)
{
  if (pos > size_)
  {
    throw std::out_of_range("Pos more than size");
  }
  Vector< T > cpy(size_ + k);
  try
  {
    for (; cpy.size_ < pos; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
    }
    for (; cpy.size_ < size_; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_ + k) T((*this)[cpy.size_]);
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < pos; ++i)
    {
      (cpy.data_ + i)->~T();
    }
    for (size_t i = pos; i < cpy.size_; ++i)
    {
      (cpy.data_ + i + k)->~T();
    }
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::pushBackImpl(const T& val)
{
  new (data_ + size_++) T(val);
}

template< class T >
void shirokov::Vector< T >::erase(VIter< T > pos, size_t count)
{
  erase(pos.pos_, pos.pos_ + count);
}

template< class T >
void shirokov::Vector< T >::erase(VIter< T > pos)
{
  erase(pos.pos_);
}

template< class T >
void shirokov::Vector< T >::erase(VIter< T > beg, VIter< T > end)
{
  erase(beg.pos_, end.pos_);
}

template< class T >
void shirokov::Vector< T >::insert(const T& value, size_t count, VIter< T > pos)
{
  Vector< T > cpy(size_ + count);
  try
  {
    for (; cpy.size_ < pos.pos_; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this).at(cpy.size_));
    }
    for (size_t i = 0; i < count; ++i)
    {
      new (cpy.data_ + cpy.size_++) T(value);
    }
    for (; pos != (*this).end(); ++pos)
    {
      new (cpy.data_ + cpy.size_++) T(*pos);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::insert(VIter< T > pos, const T& val)
{
  insert(pos.pos_, val);
}

template< class T >
void shirokov::clear(T* data, size_t to_pos)
{
  for (size_t j = 0; j < to_pos; ++j)
  {
    (data + j)->~T();
  }
}

template< class T >
void shirokov::Vector< T >::reserve(size_t cap)
{
  if (capacity_ >= cap)
  {
    return;
  }
  T* d = static_cast< T* >(::operator new(sizeof(T) * cap));
  size_t i = 0;
  try
  {
    for (; i < getSize(); ++i)
    {
      new (d + i) T(std::move(data_[i]));
    }
  }
  catch (...)
  {
    clear(d, i);
    ::operator delete(d);
    throw;
  }
  clear(data_, size_);
  ::operator delete(data_);
  data_ = d;
  capacity_ = cap;
}

template< class T >
shirokov::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  try
  {
    for (auto&& v : il)
    {
      new (data_ + size_++) T(std::move(v));
    }
  }
  catch (...)
  {
    clear(data_, size_);
    throw;
  }
}

template< class T >
template< class FwdIterator >
void shirokov::Vector< T >::insert(VIter< T > pos, FwdIterator beg, FwdIterator end)
{
  size_t count = 0;
  auto cpyBeg = beg;
  while (cpyBeg != end)
  {
    ++count;
    ++cpyBeg;
  }
  Vector< T > cpy(size_ + count);
  try
  {
    for (; cpy.size_ < pos.pos_; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this).at(cpy.size_));
    }
    for (; beg != end; ++beg)
    {
      new (cpy.data_ + cpy.size_++) T(*beg);
    }
    for (; pos != (*this).end(); ++pos)
    {
      new (cpy.data_ + cpy.size_++) T(*pos);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
shirokov::VCIter< T > shirokov::VCIter< T >::operator-(size_t i) noexcept
{
  VCIter< T > iter(v_, pos_ - i);
  return iter;
}

template< class T >
shirokov::VCIter< T > shirokov::VCIter< T >::operator+(size_t i) noexcept
{
  VCIter< T > iter(v_, pos_ + i);
  return iter;
}

template< class T >
shirokov::VIter< T > shirokov::VIter< T >::operator-(size_t i) noexcept
{
  VIter< T > iter(v_, pos_ - i);
  return iter;
}

template< class T >
shirokov::VIter< T > shirokov::VIter< T >::operator+(size_t i) noexcept
{
  VIter< T > iter(v_, pos_ + i);
  return iter;
}

template< class T >
shirokov::VCIter< T >& shirokov::VCIter< T >::operator--() noexcept
{
  --pos_;
  return *this;
}

template< class T >
shirokov::VIter< T >& shirokov::VIter< T >::operator--() noexcept
{
  --pos_;
  return *this;
}

template< class T >
shirokov::VCIter< T >& shirokov::VCIter< T >::operator++() noexcept
{
  ++pos_;
  return *this;
}

template< class T >
shirokov::VIter< T >& shirokov::VIter< T >::operator++() noexcept
{
  ++pos_;
  return *this;
}

template< class T >
const T& shirokov::VCIter< T >::operator*()
{
  return v_.at(pos_);
}

template< class T >
T& shirokov::VIter< T >::operator*()
{
  return v_.at(pos_);
}

template< class T >
bool shirokov::VCIter< T >::operator==(const VCIter< T >& rhs) const noexcept
{
  return (std::addressof(this->v_) == std::addressof(rhs.v_)) && (this->pos_ == rhs.pos_);
}

template< class T >
bool shirokov::VCIter< T >::operator!=(const VCIter< T >& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
bool shirokov::VIter< T >::operator==(const VIter< T >& rhs) const noexcept
{
  return (std::addressof(this->v_) == std::addressof(rhs.v_)) && (this->pos_ == rhs.pos_);
}

template< class T >
bool shirokov::VIter< T >::operator!=(const VIter< T >& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class T >
shirokov::VCIter< T > shirokov::Vector< T >::cbegin() const
{
  return VCIter< T >(*this, 0);
}

template< class T >
shirokov::VCIter< T > shirokov::Vector< T >::begin() const
{
  return cbegin();
}

template< class T >
shirokov::VIter< T > shirokov::Vector< T >::begin()
{
  return VIter< T >(*this, 0);
}

template< class T >
shirokov::VCIter< T > shirokov::Vector< T >::cend() const
{
  return VCIter< T >(*this, size_);
}

template< class T >
shirokov::VCIter< T > shirokov::Vector< T >::end() const
{
  return cend();
}

template< class T >
shirokov::VIter< T > shirokov::Vector< T >::end()
{
  return VIter< T >(*this, size_);
}

template< class T >
shirokov::VCIter< T >::VCIter(const Vector< T >& v, size_t pos):
  v_(v),
  pos_(pos)
{}

template< class T >
shirokov::VIter< T >::VIter(Vector< T >& v, size_t pos):
  v_(v),
  pos_(pos)
{}

template< class T >
void shirokov::Vector< T >::erase(size_t beg, size_t end)
{
  if (!size_)
  {
    throw std::out_of_range("Erasing from empty vector");
  }
  if (end > size_)
  {
    throw std::range_error("End more than size");
  }
  if (beg > end)
  {
    throw std::range_error("Begin more than end");
  }
  size_t rem = end - beg;
  Vector< T > cpy(size_ - rem);
  try
  {
    for (; cpy.size_ < beg; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
    }
    for (; cpy.size_ < size_ - rem; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ + rem]);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::insert(size_t i, const Vector< T >& rhs, size_t beg, size_t end)
{
  if (i > size_)
  {
    throw std::out_of_range("Index more than size");
  }
  if (end > rhs.getSize())
  {
    throw std::range_error("End index more than size of rhs");
  }
  if (end < beg)
  {
    throw std::range_error("End less than begin");
  }
  size_t add = end - beg;
  Vector< T > cpy(size_ + add);
  try
  {
    for (; cpy.size_ < i; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
    }
    for (; cpy.size_ < i + add; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T(rhs[beg + cpy.size_ - i]);
    }
    for (; cpy.size_ < size_ + add; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ - add]);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::erase(size_t i)
{
  if (i >= size_)
  {
    throw std::out_of_range("Index is more than size");
  }
  Vector< T > cpy(size_ - 1);
  try
  {
    for (; cpy.size_ < i; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
    }
    for (; cpy.size_ < size_ - 1; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ + 1]);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::insert(size_t i, const T& val)
{
  if (i > size_)
  {
    throw std::out_of_range("Index is more than size");
  }
  Vector< T > cpy(size_ + 1);
  try
  {
    for (; cpy.size_ < i; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
    }
    new (cpy.data_ + cpy.size_++) T(val);
    for (; cpy.size_ < size_ + 1; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ - 1]);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::popBack()
{
  if (size_ == 0)
  {
    throw std::out_of_range("Vector is empty");
  }
  (data_ + --size_)->~T();
}

template< class T >
void shirokov::Vector< T >::popFront()
{
  if (size_ == 0)
  {
    throw std::out_of_range("Vector is empty");
  }
  Vector< T > cpy(size_ - 1);
  try
  {
    for (; cpy.size_ < size_ - 1; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ + 1]);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
size_t shirokov::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void shirokov::Vector< T >::pushFront(const T& val)
{
  Vector< T > cpy(size_ + 1);
  try
  {
    new (cpy.data_) T(val);
    for (cpy.size_ = 1; cpy.size_ < size_ + 1; ++cpy.size_)
    {
      new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ - 1]);
    }
  }
  catch (...)
  {
    clear(cpy.data_, cpy.size_);
    ::operator delete(cpy.data_);
    throw;
  }
  swap(cpy);
}

template< class T >
void shirokov::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(rhs.data_, data_);
  std::swap(rhs.size_, size_);
  std::swap(rhs.capacity_, capacity_);
}

template< class T >
T& shirokov::Vector< T >::operator[](size_t id) noexcept
{
  const Vector< T >* cthis = this;
  const T& ret = (*cthis)[id];
  return const_cast< T& >(ret);
}

template< class T >
const T& shirokov::Vector< T >::operator[](size_t id) const noexcept
{
  assert(id < getSize());
  return data_[id];
}

template< class T >
T& shirokov::Vector< T >::at(size_t id)
{
  const Vector< T >* cthis = this;
  const T& ret = cthis->at(id);
  return const_cast< T& >(ret);
}

template< class T >
const T& shirokov::Vector< T >::at(size_t id) const
{
  if (getSize() && id < getSize())
  {
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
}

template< class T >
size_t shirokov::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
shirokov::Vector< T >::Vector(size_t k):
  data_(static_cast< T* >(::operator new(sizeof(T) * k))),
  size_(0),
  capacity_(k)
{}

template< class T >
shirokov::Vector< T >& shirokov::Vector< T >::operator=(const Vector< T >& rhs)
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }
  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template< class T >
shirokov::Vector< T >& shirokov::Vector< T >::operator=(Vector< T >&& rhs) noexcept
{
  if (this == std::addressof(rhs))
  {
    return *this;
  }
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
shirokov::Vector< T >::Vector(const Vector< T >& rhs):
  data_(static_cast< T* >(::operator new(rhs.getSize() * sizeof(T)))),
  size_(0),
  capacity_(rhs.getSize())
{
  try
  {
    for (; size_ < rhs.getSize(); ++size_)
    {
      new (data_ + size_) T(rhs[size_]);
    }
  }
  catch (...)
  {
    clear(data_, size_);
    ::operator delete(data_);
    throw;
  }
}

template< class T >
shirokov::Vector< T >::Vector(Vector< T >&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capacity_(rhs.capacity_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.capacity_ = 0;
}

template< class T >
bool shirokov::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template< class T >
shirokov::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
shirokov::Vector< T >::Vector::~Vector()
{
  clear(data_, size_);
  ::operator delete(data_);
}

template< class T >
void shirokov::Vector< T >::pushBack(const T& val)
{
  if (size_ == capacity_)
  {
    size_t newCap = (capacity_ == 0 ? 1 : capacity_ * 2);
    reserve(newCap);
  }
  pushBackImpl(val);
}

#endif
