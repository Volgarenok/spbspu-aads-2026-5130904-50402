#include "hashers.hpp"

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>

std::size_t samarin::StringHash::operator()(const std::string& key) const
{
  boost::hash2::siphash_64 hasher;
  boost::hash2::hash_append(hasher, {}, key);
  return static_cast< std::size_t >(hasher.result());
}

std::size_t samarin::PairHash::operator()(const std::pair< std::string, std::string >& key) const
{
  boost::hash2::siphash_64 hasher;
  boost::hash2::hash_append(hasher, {}, key.first);
  boost::hash2::hash_append(hasher, {}, key.second);
  return static_cast< std::size_t >(hasher.result());
}
