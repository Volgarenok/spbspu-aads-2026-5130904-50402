#ifndef HASHER_HPP
#define HASHER_HPP
#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <array>

namespace novikov
{
  template< class T >
  class SipHash
  {
  public:
    SipHash();
    size_t operator()(const T& v) const;

  private:
    boost::hash2::siphash_64 h_;
  };
}

template< class T >
novikov::SipHash< T >::SipHash()
{
  std::array< unsigned char, 16 > seed = {
      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
      0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
  };

  h_ = boost::hash2::siphash_64(seed.data(), seed.size());
}

template< class T >
size_t novikov::SipHash< T >::operator()(const T& v) const
{
  boost::hash2::siphash_64 h(h_);
  boost::hash2::hash_append(h, boost::hash2::default_flavor{}, v);
  return boost::hash2::get_integral_result< size_t >(h);
}

#endif
