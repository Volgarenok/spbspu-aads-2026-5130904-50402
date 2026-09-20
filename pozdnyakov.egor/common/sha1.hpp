#ifndef SHA1_HPP
#define SHA1_HPP

#include <cstddef>
#include <string>
#include <utility>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/sha1.hpp>

namespace pozdnyakov
{
  namespace hash
  {
    struct GraphEdgeSHA1
    {
      std::size_t operator()(const std::pair< std::string, std::string > &p) const
      {
        boost::hash2::sha1_160 hasher;
        std::string combined = p.first + ":" + p.second;
        boost::hash2::hash_append(hasher, {}, combined);

        auto digest = hasher.result();
        std::size_t hashValue = 0;

        for (std::size_t i = 0; i < digest.size(); ++i) {
          hashValue ^= static_cast< std::size_t >(digest[i]) << ((i % sizeof(std::size_t)) * 8);
        }

        return hashValue;
      }
    };
  }
}

#endif
