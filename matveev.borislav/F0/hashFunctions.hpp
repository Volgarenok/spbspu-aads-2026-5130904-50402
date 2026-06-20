#ifndef MATVEEV_GAMEWORLD_HASH_FUNCTIONS_HPP
#define MATVEEV_GAMEWORLD_HASH_FUNCTIONS_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace matveev
{
struct StringHash
{
  std::size_t operator()(const std::string& value) const
  {
    std::uint64_t hash = 1469598103934665603ull;

    for (std::size_t i = 0; i < value.size(); ++i)
    {
      hash ^= static_cast< std::uint64_t >(static_cast< unsigned char >(value[i]));
      hash *= 1099511628211ull;
    }

    return static_cast< std::size_t >(hash);
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
