#ifndef SHA1_HASH_HPP
#define SHA1_HASH_HPP

#include <boost/hash2/sha1.hpp>
#include <boost/hash2/hash_append.hpp>
#include <string>
#include <utility>
#include <cstddef>

namespace petrov {

  template< class T >
  struct SHA1Hasher {
    size_t operator()(const T& value) const
    {
      boost::hash2::sha1_128 hasher;
      boost::hash2::hash_append(hasher, {}, value);
      auto digest = hasher.result();
      size_t hash = 0;
      for (size_t i = 0; i < digest.size(); ++i) {
        hash ^= static_cast< size_t >(digest[i]) << ((i % sizeof(size_t)) * 8);
      }
      return hash;
    }
  };

}
#endif

