#include "cuckoo_table.hpp"

std::size_t samarin::detail::cuckooMix(std::size_t x)
{
  x += 0x9e3779b97f4a7c15ull;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
  x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
  x = x ^ (x >> 31);
  return x;
}
