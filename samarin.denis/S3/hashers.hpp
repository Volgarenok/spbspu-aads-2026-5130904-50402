#ifndef HASHERS_HPP
#define HASHERS_HPP

#include <cstddef>
#include <string>
#include <utility>

namespace samarin {

  struct StringHash {
    std::size_t operator()(const std::string& key) const;
  };

  struct PairHash {
    std::size_t operator()(const std::pair< std::string, std::string >& key) const;
  };

}

#endif
