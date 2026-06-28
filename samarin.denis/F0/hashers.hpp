#ifndef HASHERS_HPP
#define HASHERS_HPP

#include <cstddef>
#include <string>

namespace samarin {

  std::size_t hashBytes(const char* data, std::size_t size);

  struct StringHash {
    std::size_t operator()(const std::string& key) const;
  };

}

#endif
