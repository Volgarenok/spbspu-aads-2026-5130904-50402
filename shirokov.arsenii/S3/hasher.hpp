#ifndef HASHER_HPP
#define HASHER_HPP
#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/sha1.hpp>

namespace shirokov
{
  template < class T >
  class SHA1
  {
  public:
    SHA1();
    size_t operator()(const T& v) const;

  private:
    uint64_t seed_ = 0x8badf00d1badb002;
    boost::hash2::sha1_160 h_;
  };
}

template < class T >
shirokov::SHA1< T >::SHA1():
  h_(seed_)
{}

template < class T >
size_t shirokov::SHA1< T >::operator()(const T& v) const
{
  boost::hash2::sha1_160 h(h_);
  boost::hash2::hash_append(h, boost::hash2::default_flavor{}, v);
  return boost::hash2::get_integral_result< size_t >(h);
}

#endif
