#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace dirko
{
  template < class T >
  class Vector;

  template < class T >
  struct VIter
  {
    explicit VIter(Vector< T > &v, size_t pos);
    bool operator==(const VIter< T > &) const noexcept;
    bool operator!=(const VIter< T > &) const noexcept;
    VIter< T > &operator++() noexcept;
    VIter< T > &operator--() noexcept;
    VIter< T > operator+(size_t i) noexcept;
    VIter< T > operator-(size_t i) noexcept;
    T &operator*();

  private:
    Vector< T > &v_;
    size_t pos_;
    friend class Vector< T >;
  };

  template < class T >
  struct VCIter
  {
    explicit VCIter(const Vector< T > &v, size_t pos);
    bool operator==(const VCIter< T > &) const noexcept;
    bool operator!=(const VCIter< T > &) const noexcept;
    VCIter< T > &operator++() noexcept;
    VCIter< T > &operator--() noexcept;
    VCIter< T > operator+(size_t i) noexcept;
    VCIter< T > operator-(size_t i) noexcept;
    const T &operator*();

  private:
    const Vector< T > &v_;
    size_t pos_;
    friend class Vector< T >;
  };
  template < class T >
  class Vector
  {
  public:
    Vector();
    Vector(const Vector< T > &);
    Vector(Vector< T > &&) noexcept;
    explicit Vector(std::initializer_list< T > il);

    Vector< T > &operator=(const Vector< T > &);
    Vector< T > &operator=(Vector< T > &&) noexcept;
    T &operator[](size_t id) noexcept;
    const T &operator[](size_t id) const noexcept;

    ~Vector();

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapasity() const noexcept;
    void reserve(size_t cap);
    void shrinkToFit();

    void swap(Vector< T > &rhs) noexcept;

    void pushFront(const T &);
    void popFront();
    void pushBack(const T &);
    void popBack();

    T &at(size_t id);
    const T &at(size_t id) const;

    void insert(size_t i, const T &val);
    void erase(size_t i);
    void insert(size_t i, const Vector< T > &rhs, size_t beg, size_t end);
    void erase(size_t beg, size_t end);

    void insert(VIter< T > pos, VIter< T > beg, VIter< T > end);
    void insert(VIter< T > pos, const T &value);
    void insert(const T &value, size_t count, VIter< T > pos);
    void erase(VIter< T > beg, VIter< T > end);
    void erase(VIter< T > pos);
    void erase(VIter< T > pos, size_t count);

    VIter< T > begin();
    VCIter< T > cbegin() const;
    VIter< T > end();
    VCIter< T > cend() const;

  private:
    T *data_;
    size_t size_, capasity_;
    explicit Vector(size_t k);
    void pushBackImpl(const T &);
    void reserve(size_t pos, size_t k);
  };
  template < class T >
  void clear(T *data, size_t to_pos);
}

template < class T >
dirko::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capasity_(0)
{}

template < class T >
dirko::Vector< T >::Vector(const Vector< T > &rhs):
  Vector(rhs.getSize())
{
  for (; size_ < rhs.getSize(); ++size_) {
    new (data_ + size_) T(rhs[size_]);
  }
}

template < class T >
dirko::Vector< T >::Vector(Vector< T > &&rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  capasity_(rhs.capasity_)
{
  rhs.data_ = nullptr;
}

template < class T >
dirko::Vector< T > &dirko::Vector< T >::operator=(const Vector< T > &rhs)
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector< T > cpy(rhs);
  swap(cpy);
  return *this;
}

template < class T >
dirko::Vector< T > &dirko::Vector< T >::operator=(Vector< T > &&rhs) noexcept
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector< T > cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template < class T >
T &dirko::Vector< T >::operator[](size_t id) noexcept
{
  assert(id < getSize());
  return data_[id];
}

template < class T >
const T &dirko::Vector< T >::operator[](size_t id) const noexcept
{
  assert(id < getSize());
  return data_[id];
}
template < class T >
dirko::Vector< T >::Vector(size_t k):
  data_(static_cast< T * >(::operator new(sizeof(T) * k))),
  size_(0),
  capasity_(k)
{}

template < class T >
dirko::Vector< T >::~Vector()
{
  clear(data_, size_);
}

template < class T >
bool dirko::Vector< T >::isEmpty() const noexcept
{
  return !size_;
}

template < class T >
size_t dirko::Vector< T >::getSize() const noexcept
{
  return size_;
}

template < class T >
size_t dirko::Vector< T >::getCapasity() const noexcept
{
  return capasity_;
}

template < class T >
void dirko::Vector< T >::swap(Vector< T > &rhs) noexcept
{
  std::swap(rhs.data_, data_);
  std::swap(rhs.size_, size_);
  std::swap(rhs.capasity_, capasity_);
}

template < class T >
void dirko::Vector< T >::pushFront(const T &val)
{
  Vector< T > cpy(size_ + 1);
  new (cpy.data_) T(val);
  for (cpy.size_ = 1; cpy.size_ < size_ + 1; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ - 1]);
  }
  swap(cpy);
}

template < class T >
void dirko::Vector< T >::popFront()
{
  if (size_ < 1) {
    throw std::out_of_range("empty vector");
  }
  Vector< T > cpy(size_ - 1);
  for (; cpy.size_ < size_ - 1; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ + 1]);
  }
  swap(cpy);
}

template < class T >
void dirko::Vector< T >::pushBack(const T &val)
{
  if (size_ == capasity_) {
    size_t newCap = (capasity_ == 0 ? 1 : capasity_ * 2);
    reserve(newCap);
  }
  pushBackImpl(val);
}

template < class T >
void dirko::Vector< T >::pushBackImpl(const T &val)
{
  new (data_ + size_++) T(val);
}

template < class T >
void dirko::Vector< T >::popBack()
{

  if (size_ < 1) {
    throw std::out_of_range("empty vector");
  }
  (data_ + --size_)->~T();
}

template < class T >
T &dirko::Vector< T >::at(size_t id)
{
  if (id < getSize()) {
    return data_[id];
  }
  throw std::out_of_range("bad id");
}
template < class T >
const T &dirko::Vector< T >::at(size_t id) const
{
  if (id < getSize()) {
    return data_[id];
  }
  throw std::out_of_range("bad id");
}

template < class T >
void dirko::Vector< T >::insert(size_t i, const T &val)
{
  if (i > size_) {
    throw std::out_of_range("index out of range");
  }
  Vector< T > cpy(size_ + 1);
  for (; cpy.size_ < i; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
  }
  new (cpy.data_ + cpy.size_++) T(val);
  for (; cpy.size_ < size_ + 1; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ - 1]);
  }
  swap(cpy);
}

template < class T >
void dirko::Vector< T >::erase(size_t i)
{
  if (i >= size_) {
    throw std::out_of_range("index out of range");
  }
  Vector< T > cpy(size_ - 1);
  for (; cpy.size_ < i; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
  }
  for (; cpy.size_ < size_ - 1; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ + 1]);
  }
  swap(cpy);
}

template < class T >
void dirko::Vector< T >::insert(size_t i, const Vector< T > &rhs, size_t beg, size_t end)
{
  if (i > size_) {
    throw std::out_of_range("index out of range");
  }
  if (end > rhs.getSize()) {
    throw std::range_error("end index more than size of rhs");
  }
  if (end < beg) {
    throw std::range_error("end less than begin");
  }
  size_t toAdd = end - beg;
  Vector< T > cpy(size_ + toAdd);
  for (; cpy.size_ < i; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
  }
  for (; cpy.size_ < i + toAdd; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T(rhs[beg + cpy.size_ - i]);
  }
  for (; cpy.size_ < size_ + toAdd; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ - toAdd]);
  }
  swap(cpy);
}
template < class T >
void dirko::Vector< T >::erase(size_t beg, size_t end)
{
  if (!size_) {
    throw std::out_of_range("empty vector");
  }
  if (end > size_) {
    throw std::range_error("end is greater than size");
  }
  if (beg > end) {
    throw std::range_error("begin is greater than end");
  }
  size_t toRemove = end - beg;
  Vector< T > cpy(size_ - toRemove);
  for (; cpy.size_ < beg; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
  }
  for (; cpy.size_ < size_ - toRemove; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_ + toRemove]);
  }
  swap(cpy);
}

template < class T >
dirko::VCIter< T > &dirko::VCIter< T >::operator--() noexcept
{
  --pos_;
  return *this;
}

template < class T >
dirko::VIter< T > &dirko::VIter< T >::operator--() noexcept
{
  --pos_;
  return *this;
}

template < class T >
dirko::VCIter< T > &dirko::VCIter< T >::operator++() noexcept
{
  ++pos_;
  return *this;
}

template < class T >
dirko::VIter< T > &dirko::VIter< T >::operator++() noexcept
{
  ++pos_;
  return *this;
}

template < class T >
const T &dirko::VCIter< T >::operator*()
{
  return v_.at(pos_);
}

template < class T >
T &dirko::VIter< T >::operator*()
{
  return v_.at(pos_);
}

template < class T >
bool dirko::VCIter< T >::operator==(const VCIter< T > &rhs) const noexcept
{
  return (std::addressof(this->v_) == std::addressof(rhs.v_)) && (this->pos_ == rhs.pos_);
}

template < class T >
bool dirko::VCIter< T >::operator!=(const VCIter< T > &rhs) const noexcept
{
  return !(*this == rhs);
}

template < class T >
bool dirko::VIter< T >::operator==(const VIter< T > &rhs) const noexcept
{
  return (std::addressof(this->v_) == std::addressof(rhs.v_)) && (this->pos_ == rhs.pos_);
}

template < class T >
bool dirko::VIter< T >::operator!=(const VIter< T > &rhs) const noexcept
{
  return !(*this == rhs);
}

template < class T >
dirko::VCIter< T > dirko::Vector< T >::cbegin() const
{
  return VCIter< T >(*this, 0);
}

template < class T >
dirko::VIter< T > dirko::Vector< T >::begin()
{
  return VIter< T >(*this, 0);
}

template < class T >
dirko::VCIter< T > dirko::Vector< T >::cend() const
{
  return VCIter< T >(*this, size_);
}

template < class T >
dirko::VIter< T > dirko::Vector< T >::end()
{
  return VIter< T >(*this, size_);
}

template < class T >
dirko::VCIter< T >::VCIter(const Vector< T > &v, size_t pos):
  v_(v),
  pos_(pos)
{}

template < class T >
dirko::VIter< T >::VIter(Vector< T > &v, size_t pos):
  v_(v),
  pos_(pos)
{}
template < class T >
dirko::Vector< T >::Vector(std::initializer_list< T > il):
  Vector(il.size())
{
  for (auto &&v : il) {
    new (data_ + size_++) T(std::move(v));
  }
}

template < class T >
void dirko::Vector< T >::reserve(size_t cap)
{
  if (capasity_ >= cap) {
    return;
  }
  T *d = static_cast< T * >(::operator new(sizeof(T) * cap));
  size_t i = 0;
  for (; i < getSize(); ++i) {
    new (d + i) T(std::move(data_[i]));
  }
  clear(data_, size_);
  data_ = d;
  capasity_ = cap;
}

template < class T >
void dirko::clear(T *data, size_t count)
{
  for (size_t j = 0; j < count; ++j) {
    (data + j)->~T();
  }
  ::operator delete(data);
}
template < class T >
void dirko::Vector< T >::shrinkToFit()
{
  Vector< T > cpy(*this);
  swap(cpy);
}
template < class T >
void dirko::Vector< T >::reserve(size_t pos, size_t k)
{
  if (pos > size_) {
    throw std::out_of_range("Pos more than size");
  }
  Vector< T > cpy(size_ + k);
  for (; cpy.size_ < pos; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this)[cpy.size_]);
  }
  for (; cpy.size_ < size_; ++cpy.size_) {
    new (cpy.data_ + cpy.size_ + k) T((*this)[cpy.size_]);
  }
  swap(cpy);
}
template < class T >
dirko::VCIter< T > dirko::VCIter< T >::operator-(size_t i) noexcept
{
  VCIter< T > iter(v_, pos_ - i);
  return iter;
}

template < class T >
dirko::VCIter< T > dirko::VCIter< T >::operator+(size_t i) noexcept
{
  VCIter< T > iter(v_, pos_ + i);
  return iter;
}

template < class T >
dirko::VIter< T > dirko::VIter< T >::operator-(size_t i) noexcept
{
  VIter< T > iter(v_, pos_ - i);
  return iter;
}

template < class T >
dirko::VIter< T > dirko::VIter< T >::operator+(size_t i) noexcept
{
  VIter< T > iter(v_, pos_ + i);
  return iter;
}
template < class T >
void dirko::Vector< T >::erase(VIter< T > pos, size_t count)
{
  erase(pos.pos_, pos.pos_ + count);
}

template < class T >
void dirko::Vector< T >::erase(VIter< T > pos)
{
  erase(pos.pos_);
}

template < class T >
void dirko::Vector< T >::erase(VIter< T > beg, VIter< T > end)
{
  erase(beg.pos_, end.pos_);
}
template < class T >
void dirko::Vector< T >::insert(const T &value, size_t count, VIter< T > pos)
{
  Vector< T > cpy(size_ + count);
  for (; cpy.size_ < pos.pos_; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this).at(cpy.size_));
  }
  for (size_t i = 0; i < count; ++i) {
    new (cpy.data_ + cpy.size_++) T(value);
  }
  for (; pos != (*this).end(); ++pos) {
    new (cpy.data_ + cpy.size_++) T(*pos);
  }
  swap(cpy);
}

template < class T >
void dirko::Vector< T >::insert(VIter< T > pos, const T &val)
{
  insert(pos.pos_, val);
}
template < class T >
void dirko::Vector< T >::insert(VIter< T > pos, VIter< T > beg, VIter< T > end)
{
  size_t count = 0;
  VIter< T > cpyBeg = beg;
  while (cpyBeg != end) {
    ++count;
    ++cpyBeg;
  }
  Vector< T > cpy(size_ + count);
  for (; cpy.size_ < pos.pos_; ++cpy.size_) {
    new (cpy.data_ + cpy.size_) T((*this).at(cpy.size_));
  }
  for (; beg != end; ++beg) {
    new (cpy.data_ + cpy.size_++) T(*beg);
  }
  for (; pos != (*this).end(); ++pos) {
    new (cpy.data_ + cpy.size_++) T(*pos);
  }
  swap(cpy);
}
#endif
