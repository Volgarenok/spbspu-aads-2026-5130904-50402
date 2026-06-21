#ifndef HASHER_HPP
#define HASHER_HPP

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>

namespace khalikov
{
  template < class T > class SipHash
  {
  public:
    SipHash();
    size_t operator()(const T &v) const;

  private:
    uint64_t seed_[2] = {0x0706050403020100ULL, 0x0f0e0d0c0b0a0908ULL};
    boost::hash2::siphash_64 h_;
  };
}

template < class T >
khalikov::SipHash< T >::SipHash():
  h_(static_cast< const void * >(seed_), 16)
{}

template < class T > size_t khalikov::SipHash< T >::operator()(const T &v) const
{
  boost::hash2::siphash_64 h(h_);
  boost::hash2::hash_append(h, boost::hash2::default_flavor{}, v);
  return boost::hash2::get_integral_result< size_t >(h);
}

#endif
