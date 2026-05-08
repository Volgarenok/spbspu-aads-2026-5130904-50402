#ifndef HASHITER_HPP
#define HASHITER_HPP

#include <cstddef>

namespace sogdanov
{

  template <class T>
  class HashIter
  {
    T *tbl_;
    size_t cap_;
    size_t idx_;

  public:
    HashIter(T *t, size_t c, size_t i);

    void advance();
    bool operator!=(const HashIter &o) const;
    HashIter &operator++();

    T &operator*();
  };

}

template <class T>
sogdanov::HashIter<T>::HashIter(T *t, size_t c, size_t i)
    : tbl_(t), cap_(c), idx_(i)
{
  advance();
}

template <class T>
void sogdanov::HashIter<T>::advance()
{
  while (idx_ < cap_ && !tbl_[idx_].is_occupied())
  {
    idx_++;
  }
}

template <class T>
bool sogdanov::HashIter<T>::operator!=(const HashIter &o) const
{
  return idx_ != o.idx_;
}

template <class T>
sogdanov::HashIter<T> &sogdanov::HashIter<T>::operator++()
{
  idx_++;
  advance();
  return *this;
}

template <class T>
T &sogdanov::HashIter<T>::operator*()
{
  return tbl_[idx_];
}

#endif
