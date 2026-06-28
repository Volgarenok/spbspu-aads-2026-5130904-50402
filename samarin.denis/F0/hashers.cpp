#include "hashers.hpp"

std::size_t samarin::hashBytes(const char* data, std::size_t size)
{
  const std::size_t offset = 14695981039346656037ull;
  const std::size_t prime = 1099511628211ull;
  std::size_t hash = offset;
  for (std::size_t i = 0; i < size; ++i) {
    hash ^= static_cast< unsigned char >(data[i]);
    hash *= prime;
  }
  return hash;
}

std::size_t samarin::StringHash::operator()(const std::string& key) const
{
  return hashBytes(key.data(), key.size());
}
