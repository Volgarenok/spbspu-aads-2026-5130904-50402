#ifndef VEC_ITER_HPP
#define VEC_ITER_HPP
#include <cstddef>
namespace sogdanov
{
  template < class T >
  class Vector;
  template < class T >
  class VIter
  {
    friend class Vector< T >;
    Vector< T > *vec_;
    size_t idx_;

  public:
    VIter();
    VIter(Vector< T > *p, size_t idx);
    T &operator*() const;
    T &operator[](long long n) const;
    VIter &operator++();
    VIter operator++(int);
    VIter &operator--();
    VIter operator--(int);
    VIter operator+(long long n) const;
    VIter operator-(long long n) const;
    VIter &operator+=(long long n);
    VIter &operator-=(long long n);
    long long operator-(const VIter &other) const;
    bool operator==(const VIter &other) const;
    bool operator!=(const VIter &other) const;
    bool operator<(const VIter &other) const;
    bool operator>(const VIter &other) const;
    bool operator<=(const VIter &other) const;
    bool operator>=(const VIter &other) const;
  };
  template < class T >
  class VCIter
  {
    friend class Vector< T >;
    const Vector< T > *vec_;
    size_t idx_;

  public:
    VCIter();
    VCIter(const Vector< T > *p, size_t idx);
    VCIter(const VIter< T > &other);
    const T &operator*() const;
    const T &operator[](long long n) const;
    VCIter &operator++();
    VCIter operator++(int);
    VCIter &operator--();
    VCIter operator--(int);
    VCIter operator+(long long n) const;
    VCIter operator-(long long n) const;
    VCIter &operator+=(long long n);
    VCIter &operator-=(long long n);
    long long operator-(const VCIter &other) const;
    bool operator==(const VCIter &other) const;
    bool operator!=(const VCIter &other) const;
    bool operator<(const VCIter &other) const;
    bool operator>(const VCIter &other) const;
    bool operator<=(const VCIter &other) const;
    bool operator>=(const VCIter &other) const;
  };
  template < class T >
  VIter< T > operator+(long long, const VIter< T > &);
  template < class T >
  VCIter< T > operator+(long long, const VCIter< T > &);
}

template < class T >
sogdanov::VIter< T >::VIter():
  vec_(nullptr),
  idx_(0)
{}

template < class T >
sogdanov::VIter< T >::VIter(sogdanov::Vector< T > *p, size_t idx):
  vec_(p),
  idx_(idx)
{}

template < class T >
T &sogdanov::VIter< T >::operator*() const
{
  return vec_->data_[idx_];
}

template < class T >
T &sogdanov::VIter< T >::operator[](long long n) const
{
  long long new_idx = static_cast< long long >(idx_) + n;
  return vec_->data_[static_cast< size_t >(new_idx)];
}

template < class T >
sogdanov::VIter< T > &sogdanov::VIter< T >::operator++()
{
  ++idx_;
  return *this;
}

template < class T >
sogdanov::VIter< T > sogdanov::VIter< T >::operator++(int)
{
  VIter tmp = *this;
  ++(*this);
  return tmp;
}

template < class T >
sogdanov::VIter< T > &sogdanov::VIter< T >::operator--()
{
  --idx_;
  return *this;
}

template < class T >
sogdanov::VIter< T > sogdanov::VIter< T >::operator--(int)
{
  VIter tmp = *this;
  --(*this);
  return tmp;
}
template < class T >
sogdanov::VIter< T > sogdanov::VIter< T >::operator+(long long n) const
{
  VIter tmp = *this;
  return tmp += n;
}

template < class T >
sogdanov::VIter< T > sogdanov::VIter< T >::operator-(long long n) const
{
  VIter tmp = *this;
  return tmp -= n;
}
template < class T >
sogdanov::VIter< T > &sogdanov::VIter< T >::operator+=(long long n)
{
  idx_ = static_cast< size_t >(static_cast< long long >(idx_) + n);
  return *this;
}

template < class T >
sogdanov::VIter< T > &sogdanov::VIter< T >::operator-=(long long n)
{
  idx_ = static_cast< size_t >(static_cast< long long >(idx_) - n);
  return *this;
}

template < class T >
long long sogdanov::VIter< T >::operator-(const VIter &other) const
{
  return static_cast< long long >(idx_) - static_cast< long long >(other.idx_);
}

template < class T >
bool sogdanov::VIter< T >::operator==(const VIter &other) const
{
  return vec_ == other.vec_ && idx_ == other.idx_;
}

template < class T >
bool sogdanov::VIter< T >::operator!=(const VIter &other) const
{
  return !(*this == other);
}

template < class T >
bool sogdanov::VIter< T >::operator<(const VIter &other) const
{
  return idx_ < other.idx_;
}

template < class T >
bool sogdanov::VIter< T >::operator>(const VIter &other) const
{
  return other < *this;
}

template < class T >
bool sogdanov::VIter< T >::operator<=(const VIter &other) const
{
  return !(other < *this);
}

template < class T >
bool sogdanov::VIter< T >::operator>=(const VIter &other) const
{
  return !(*this < other);
}

template < class T >
sogdanov::VCIter< T >::VCIter():
  vec_(nullptr),
  idx_(0)
{}

template < class T >
sogdanov::VCIter< T >::VCIter(const sogdanov::Vector< T > *p, size_t idx):
  vec_(p),
  idx_(idx)
{}

template < class T >
sogdanov::VCIter< T >::VCIter(const sogdanov::VIter< T > &other):
  vec_(other.vec_),
  idx_(other.idx_)
{}

template < class T >
const T &sogdanov::VCIter< T >::operator*() const
{
  return vec_->data_[idx_];
}

template < class T >
const T &sogdanov::VCIter< T >::operator[](long long n) const
{
  long long new_idx = static_cast< long long >(idx_) + n;
  return vec_->data_[static_cast< size_t >(new_idx)];
}

template < class T >
sogdanov::VCIter< T > &sogdanov::VCIter< T >::operator++()
{
  ++idx_;
  return *this;
}

template < class T >
sogdanov::VCIter< T > sogdanov::VCIter< T >::operator++(int)
{
  VCIter tmp = *this;
  ++(*this);
  return tmp;
}

template < class T >
sogdanov::VCIter< T > &sogdanov::VCIter< T >::operator--()
{
  --idx_;
  return *this;
}

template < class T >
sogdanov::VCIter< T > sogdanov::VCIter< T >::operator--(int)
{
  VCIter tmp = *this;
  --(*this);
  return tmp;
}

template < class T >
sogdanov::VCIter< T > sogdanov::VCIter< T >::operator+(long long n) const
{
  VCIter tmp = *this;
  return tmp += n;
}

template < class T >
sogdanov::VCIter< T > sogdanov::VCIter< T >::operator-(long long n) const
{
  VCIter tmp = *this;
  return tmp -= n;
}
template < class T >
sogdanov::VCIter< T > &sogdanov::VCIter< T >::operator+=(long long n)
{
  idx_ = static_cast< size_t >(static_cast< long long >(idx_) + n);
  return *this;
}

template < class T >
sogdanov::VCIter< T > &sogdanov::VCIter< T >::operator-=(long long n)
{
  idx_ = static_cast< size_t >(static_cast< long long >(idx_) - n);
  return *this;
}
template < class T >
long long sogdanov::VCIter< T >::operator-(const VCIter &other) const
{
  return static_cast< long long >(idx_) - static_cast< long long >(other.idx_);
}

template < class T >
bool sogdanov::VCIter< T >::operator==(const VCIter &other) const
{
  return vec_ == other.vec_ && idx_ == other.idx_;
}

template < class T >
bool sogdanov::VCIter< T >::operator!=(const VCIter &other) const
{
  return !(*this == other);
}

template < class T >
bool sogdanov::VCIter< T >::operator<(const VCIter &other) const
{
  return idx_ < other.idx_;
}

template < class T >
bool sogdanov::VCIter< T >::operator>(const VCIter &other) const
{
  return other < *this;
}

template < class T >
bool sogdanov::VCIter< T >::operator<=(const VCIter &other) const
{
  return !(other < *this);
}

template < class T >
bool sogdanov::VCIter< T >::operator>=(const VCIter &other) const
{
  return !(*this < other);
}

template < class T >
sogdanov::VIter< T > sogdanov::operator+(long long n, const VIter< T > &it)
{
  return it + n;
}

template < class T >
sogdanov::VCIter< T > sogdanov::operator+(long long n, const VCIter< T > &it)
{
  return it + n;
}
#endif
