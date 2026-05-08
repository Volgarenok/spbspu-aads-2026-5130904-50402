#ifndef HASHFUNC_HPP
#define HASHFUNC_HPP

#include <cstddef>
#include <utility>

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/xxhash.hpp>

namespace sogdanov
{

  struct XXHashFunctor
  {
    template <class T>
    std::size_t operator()(const T &val) const;
  };

  template <class T>
  struct EqualFunctor
  {
    bool operator()(const T &a, const T &b) const;
  };

}

template <class T>
std::size_t sogdanov::XXHashFunctor::operator()(const T &val) const
{
  boost::hash2::xxhash_64 hash_algo;
  boost::hash2::hash_append(hash_algo, {}, val);
  return static_cast<std::size_t>(hash_algo.result());
}

template <class T>
bool sogdanov::EqualFunctor<T>::operator()(const T &a, const T &b) const
{
  return a == b;
}

#endif