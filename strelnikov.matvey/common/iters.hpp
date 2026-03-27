#ifndef ITERS_HPP
#define ITERS_HPP

#include "node.hpp"

namespace strelnikov
{
  template < class T >
  class List;
  template < class T >
  class LCIter;
  template < class T >
  class LIter
  {
  public:
    using node = Node< T >;

    LIter() noexcept;
    LIter(const LIter &) noexcept;
    LIter(LIter &&) noexcept;

    LIter &operator=(const LIter< T > &) noexcept;
    LIter &operator=(LIter &&) noexcept;

    ~LIter() noexcept = default;

    LIter(node *) noexcept;

    T &operator*() noexcept;
    T *operator->() noexcept;

    LIter &operator++();
    LIter operator++(int);

    bool operator==(const LIter &) const noexcept;
    bool operator!=(const LIter &) const noexcept;

  private:
    Node< T > *curr_;
    friend class List< T >;
    friend class LCIter< T >;
  };

  template < class T >
  class LCIter
  {
  public:
    using node = const Node< T >;

    LCIter() noexcept;
    LCIter(const LCIter &) noexcept;
    LCIter(LCIter &&) noexcept;

    LCIter &operator=(const LCIter &) noexcept;
    LCIter &operator=(LCIter &&) noexcept;

    ~LCIter() noexcept = default;

    LCIter(const LIter< T > &other) noexcept;
    LCIter(node *n) noexcept;

    const T &operator*() const noexcept;
    const T *operator->() const noexcept;

    LCIter &operator++();
    LCIter operator++(int);

    bool operator==(const LCIter &) const noexcept;
    bool operator!=(const LCIter &) const noexcept;

  private:
    const Node< T > *curr_;
    friend class List< T >;
    friend class LIter< T >;
  };

  template < class T >
  LIter< T >::LIter() noexcept:
    curr_(nullptr)
  {}

  template < class T >
  LIter< T >::LIter(const LIter &other) noexcept:
    curr_(other.curr_)
  {}

  template < class T >
  LIter< T >::LIter(LIter &&other) noexcept:
    curr_(other.curr_)
  {
    other.curr_ = nullptr;
  }

  template < class T >
  LIter< T > &LIter< T >::operator=(const LIter &other) noexcept
  {
    curr_ = other.curr_;
    return *this;
  }

  template < class T >
  LIter< T > &LIter< T >::operator=(LIter &&other) noexcept
  {
    if (this == &other) {
      return *this;
    }

    curr_ = other.curr_;
    other.curr_ = nullptr;

    return *this;
  }

  template < class T >
  LIter< T >::LIter(node *n) noexcept:
    curr_(n)
  {}

  template < class T >
  T &LIter< T >::operator*() noexcept
  {
    return curr_->val;
  }

  template < class T >
  T *LIter< T >::operator->() noexcept
  {
    return &(curr_->val);
  }

  template < class T >
  LIter< T > &LIter< T >::operator++()
  {
    if (curr_) {
      curr_ = curr_->next;
    }
    return *this;
  }

  template < class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter tmp = *this;

    if (curr_) {
      curr_ = curr_->next;
    }

    return tmp;
  }

  template < class T >
  bool LIter< T >::operator==(const LIter &other) const noexcept
  {
    return curr_ == other.curr_;
  }

  template < class T >
  bool LIter< T >::operator!=(const LIter &other) const noexcept
  {
    return !(*this == other);
  }

  template < class T >
  LCIter< T >::LCIter() noexcept:
    curr_(nullptr)
  {}

  template < class T >
  LCIter< T >::LCIter(const LCIter &other) noexcept:
    curr_(other.curr_)
  {}

  template < class T >
  LCIter< T >::LCIter(LCIter &&other) noexcept:
    curr_(other.curr_)
  {
    other.curr_ = nullptr;
  }

  template < class T >
  LCIter< T > &LCIter< T >::operator=(const LCIter &other) noexcept
  {
    curr_ = other.curr_;
    return *this;
  }

  template < class T >
  LCIter< T > &LCIter< T >::operator=(LCIter &&other) noexcept
  {
    if (this == &other) {
      return *this;
    }

    curr_ = other.curr_;
    other.curr_ = nullptr;

    return *this;
  }

  template < class T >
  LCIter< T >::LCIter(const LIter< T > &other) noexcept:
    curr_(other.curr_)
  {}

  template < class T >
  LCIter< T >::LCIter(node *n) noexcept:
    curr_(n)
  {}

  template < class T >
  const T &LCIter< T >::operator*() const noexcept
  {
    return curr_->val;
  }

  template < class T >
  const T *LCIter< T >::operator->() const noexcept
  {
    return &(curr_->val);
  }

  template < class T >
  LCIter< T > &LCIter< T >::operator++()
  {
    if (curr_) {
      curr_ = curr_->next;
    }
    return *this;
  }

  template < class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter tmp = *this;

    if (curr_) {
      curr_ = curr_->next;
    }

    return tmp;
  }

  template < class T >
  bool LCIter< T >::operator==(const LCIter &other) const noexcept
  {
    return curr_ == other.curr_;
  }

  template < class T >
  bool LCIter< T >::operator!=(const LCIter &other) const noexcept
  {
    return !(*this == other);
  }

}
#endif
