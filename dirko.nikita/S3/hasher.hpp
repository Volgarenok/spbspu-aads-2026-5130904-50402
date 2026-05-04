#ifndef HASHER_HPP
#define HASHER_HPP

#include <cstddef>
#include <boost/container_hash/hash.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>

namespace dirko
{
  template < class T, class H = boost::hash2::siphash_64 >
  struct SipHasher
  {
    size_t operator()(const T &value) const;
  };

  template < class T >
  struct PairSipHasher
  {
    size_t operator()(const std::pair< T, T > &s) const;
  };
}

template < class T, class H >
size_t dirko::SipHasher< T, H >::operator()(const T &value) const
{
  H hasher{};
  boost::hash2::hash_append(hasher, {}, value);
  return boost::hash2::get_integral_result< size_t >(hasher);
}

template < class T >
size_t dirko::PairSipHasher< T >::operator()(const std::pair< T, T > &s) const
{
  size_t hash = dirko::SipHasher< T >{}(s.first);
  boost::hash_combine(hash, s.second);
  return hash;
}

#endif
