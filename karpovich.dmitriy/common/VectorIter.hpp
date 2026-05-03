#ifndef VECTOR_ITER_HPP
#define VECTOR_ITER_HPP
#include <cstddef>
namespace karpovich
{
  using signed_size_t = long long;
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
    T &operator[](signed_size_t n) const;
    VIter &operator++();
    VIter operator++(int);
    VIter &operator--();
    VIter operator--(int);
    VIter &operator+=(signed_size_t n);
    VIter &operator-=(signed_size_t n);
    VIter operator+(signed_size_t n) const;
    VIter operator-(signed_size_t n) const;
    signed_size_t operator-(const VIter &other) const;
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
    const T &operator[](signed_size_t n) const;
    VCIter &operator++();
    VCIter operator++(int);
    VCIter &operator--();
    VCIter operator--(int);
    VCIter &operator+=(signed_size_t n);
    VCIter &operator-=(signed_size_t n);
    VCIter operator+(signed_size_t n) const;
    VCIter operator-(signed_size_t n) const;
    signed_size_t operator-(const VCIter &other) const;
    bool operator==(const VCIter &other) const;
    bool operator!=(const VCIter &other) const;
    bool operator<(const VCIter &other) const;
    bool operator>(const VCIter &other) const;
    bool operator<=(const VCIter &other) const;
    bool operator>=(const VCIter &other) const;
  };
  template < class T >
  VIter< T > operator+(signed_size_t, const VIter< T > &);
  template < class T >
  VCIter< T > operator+(signed_size_t, const VCIter< T > &);
}

template < class T >
karpovich::VIter< T >::VIter():
  vec_(nullptr),
  idx_(0)
{}

template < class T >
karpovich::VIter< T >::VIter(karpovich::Vector< T > *p, size_t idx):
  vec_(p),
  idx_(idx)
{}

template < class T >
T &karpovich::VIter< T >::operator*() const
{
  return vec_->data_[idx_];
}

template < class T >
T &karpovich::VIter< T >::operator[](karpovich::signed_size_t n) const
{
  signed_size_t new_idx = static_cast< signed_size_t >(idx_) + n;
  return vec_->data_[static_cast< size_t >(new_idx)];
}

template < class T >
karpovich::VIter< T > &karpovich::VIter< T >::operator++()
{
  ++idx_;
  return *this;
}

template < class T >
karpovich::VIter< T > karpovich::VIter< T >::operator++(int)
{
  VIter tmp = *this;
  ++(*this);
  return tmp;
}

template < class T >
karpovich::VIter< T > &karpovich::VIter< T >::operator--()
{
  --idx_;
  return *this;
}

template < class T >
karpovich::VIter< T > karpovich::VIter< T >::operator--(int)
{
  VIter tmp = *this;
  --(*this);
  return tmp;
}

template < class T >
karpovich::VIter< T > &karpovich::VIter< T >::operator+=(karpovich::signed_size_t n)
{
  idx_ = static_cast< size_t >(static_cast< signed_size_t >(idx_) + n);
  return *this;
}

template < class T >
karpovich::VIter< T > &karpovich::VIter< T >::operator-=(karpovich::signed_size_t n)
{
  idx_ = static_cast< size_t >(static_cast< signed_size_t >(idx_) - n);
  return *this;
}

template < class T >
karpovich::VIter< T > karpovich::VIter< T >::operator+(karpovich::signed_size_t n) const
{
  VIter tmp = *this;
  return tmp += n;
}

template < class T >
karpovich::VIter< T > karpovich::VIter< T >::operator-(karpovich::signed_size_t n) const
{
  VIter tmp = *this;
  return tmp -= n;
}

template < class T >
karpovich::signed_size_t karpovich::VIter< T >::operator-(const VIter &other) const
{
  return static_cast< signed_size_t >(idx_) - static_cast< signed_size_t >(other.idx_);
}

template < class T >
bool karpovich::VIter< T >::operator==(const VIter &other) const
{
  return vec_ == other.vec_ && idx_ == other.idx_;
}

template < class T >
bool karpovich::VIter< T >::operator!=(const VIter &other) const
{
  return !(*this == other);
}

template < class T >
bool karpovich::VIter< T >::operator<(const VIter &other) const
{
  return idx_ < other.idx_;
}

template < class T >
bool karpovich::VIter< T >::operator>(const VIter &other) const
{
  return other < *this;
}

template < class T >
bool karpovich::VIter< T >::operator<=(const VIter &other) const
{
  return !(other < *this);
}

template < class T >
bool karpovich::VIter< T >::operator>=(const VIter &other) const
{
  return !(*this < other);
}

template < class T >
karpovich::VCIter< T >::VCIter():
  vec_(nullptr),
  idx_(0)
{}

template < class T >
karpovich::VCIter< T >::VCIter(const karpovich::Vector< T > *p, size_t idx):
  vec_(p),
  idx_(idx)
{}

template < class T >
karpovich::VCIter< T >::VCIter(const karpovich::VIter< T > &other):
  vec_(other.vec_),
  idx_(other.idx_)
{}

template < class T >
const T &karpovich::VCIter< T >::operator*() const
{
  return vec_->data_[idx_];
}

template < class T >
const T &karpovich::VCIter< T >::operator[](karpovich::signed_size_t n) const
{
  signed_size_t new_idx = static_cast< signed_size_t >(idx_) + n;
  return vec_->data_[static_cast< size_t >(new_idx)];
}

template < class T >
karpovich::VCIter< T > &karpovich::VCIter< T >::operator++()
{
  ++idx_;
  return *this;
}

template < class T >
karpovich::VCIter< T > karpovich::VCIter< T >::operator++(int)
{
  VCIter tmp = *this;
  ++(*this);
  return tmp;
}

template < class T >
karpovich::VCIter< T > &karpovich::VCIter< T >::operator--()
{
  --idx_;
  return *this;
}

template < class T >
karpovich::VCIter< T > karpovich::VCIter< T >::operator--(int)
{
  VCIter tmp = *this;
  --(*this);
  return tmp;
}

template < class T >
karpovich::VCIter< T > &karpovich::VCIter< T >::operator+=(karpovich::signed_size_t n)
{
  idx_ = static_cast< size_t >(static_cast< signed_size_t >(idx_) + n);
  return *this;
}

template < class T >
karpovich::VCIter< T > &karpovich::VCIter< T >::operator-=(karpovich::signed_size_t n)
{
  idx_ = static_cast< size_t >(static_cast< signed_size_t >(idx_) - n);
  return *this;
}

template < class T >
karpovich::VCIter< T > karpovich::VCIter< T >::operator+(karpovich::signed_size_t n) const
{
  VCIter tmp = *this;
  return tmp += n;
}

template < class T >
karpovich::VCIter< T > karpovich::VCIter< T >::operator-(karpovich::signed_size_t n) const
{
  VCIter tmp = *this;
  return tmp -= n;
}

template < class T >
karpovich::signed_size_t karpovich::VCIter< T >::operator-(const VCIter &other) const
{
  return static_cast< signed_size_t >(idx_) - static_cast< signed_size_t >(other.idx_);
}

template < class T >
bool karpovich::VCIter< T >::operator==(const VCIter &other) const
{
  return vec_ == other.vec_ && idx_ == other.idx_;
}

template < class T >
bool karpovich::VCIter< T >::operator!=(const VCIter &other) const
{
  return !(*this == other);
}

template < class T >
bool karpovich::VCIter< T >::operator<(const VCIter &other) const
{
  return idx_ < other.idx_;
}

template < class T >
bool karpovich::VCIter< T >::operator>(const VCIter &other) const
{
  return other < *this;
}

template < class T >
bool karpovich::VCIter< T >::operator<=(const VCIter &other) const
{
  return !(other < *this);
}

template < class T >
bool karpovich::VCIter< T >::operator>=(const VCIter &other) const
{
  return !(*this < other);
}

template < class T >
karpovich::VIter< T > karpovich::operator+(karpovich::signed_size_t n, const VIter< T > &it)
{
  return it + n;
}

template < class T >
karpovich::VCIter< T > karpovich::operator+(karpovich::signed_size_t n, const VCIter< T > &it)
{
  return it + n;
}
#endif
