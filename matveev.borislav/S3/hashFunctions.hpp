#ifndef MATVEEV_HASH_FUNCTIONS_HPP
#define MATVEEV_HASH_FUNCTIONS_HPP

#include <boost/hash2/flavor.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/xxhash.hpp>

#include <cstddef>
#include <string>

namespace matveev
{
struct StringHash
{
  size_t operator()(const std::string& value) const
  {
    boost::hash2::xxhash_64 hash;
    boost::hash2::hash_append(hash, boost::hash2::default_flavor(), value);

    return static_cast< size_t >(hash.result());
  }
};

struct StringEqual
{
  bool operator()(const std::string& lhs, const std::string& rhs) const
  {
    return lhs == rhs;
  }
};
}

#endif
