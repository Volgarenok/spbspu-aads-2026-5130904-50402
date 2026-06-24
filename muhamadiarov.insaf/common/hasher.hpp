#ifndef HASHER_HPP
#define HASHER_HPP

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstddef>
#include <cstring>
#include <string>

namespace muhamadiarov
{
  template<class T>
  class SipHash
  {
  public:
      SipHash() : h_(0x8badf00d1badb002ULL) {}

      explicit SipHash(uint64_t seed) : h_(seed) {}

      size_t operator()(const T& v) const
      {
          boost::hash2::siphash_64 h(h_);
          boost::hash2::hash_append(h, boost::hash2::default_flavor{}, v);
          return boost::hash2::get_integral_result<size_t>(h);
      }
  private:
      mutable boost::hash2::siphash_64 h_;
  };
}

#endif
