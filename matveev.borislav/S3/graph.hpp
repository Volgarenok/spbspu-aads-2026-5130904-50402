#ifndef MATVEEV_GRAPH_HPP
#define MATVEEV_GRAPH_HPP

#include <boost/hash2/flavor.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/xxhash.hpp>

#include <cstddef>
#include <string>

namespace matveev
{
struct EdgeKey
{
  EdgeKey();
  EdgeKey(const std::string& from_value, const std::string& to_value);

  std::string from;
  std::string to;
};

inline EdgeKey::EdgeKey():
  from(),
  to()
{}

inline EdgeKey::EdgeKey(const std::string& from_value, const std::string& to_value):
  from(from_value),
  to(to_value)
{}

template< class Provider, class Hash, class Flavor >
void tag_invoke(
  boost::hash2::hash_append_tag const&,
  const Provider&,
  Hash& hash,
  const Flavor& flavor,
  const EdgeKey* key
)
{
  boost::hash2::hash_append(hash, flavor, key->from.size());
  boost::hash2::hash_append(hash, flavor, key->from);
  boost::hash2::hash_append(hash, flavor, key->to.size());
  boost::hash2::hash_append(hash, flavor, key->to);
}

struct EdgeHash
{
  size_t operator()(const EdgeKey& key) const
  {
    boost::hash2::xxhash_64 hash;
    boost::hash2::hash_append(hash, boost::hash2::default_flavor(), key);

    return static_cast< size_t >(hash.result());
  }
};

struct EdgeEqual
{
  bool operator()(const EdgeKey& lhs, const EdgeKey& rhs) const
  {
    return lhs.from == rhs.from && lhs.to == rhs.to;
  }
};
}

#endif
