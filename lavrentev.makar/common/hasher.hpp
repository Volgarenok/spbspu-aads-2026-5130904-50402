#ifndef HASHER_HPP
#define HASHER_HPP

#include <array>
#include <cstddef>
#include <random>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

template <class Key>
class Siphash {
  boost::hash2::siphash_64 h1_;
  boost::hash2::siphash_64 h2_;

  static std::array<unsigned char, 16> randomSeed()
  {
    std::array<unsigned char, 16> s{};
    std::random_device rd;
    for (auto &b : s) b = static_cast<unsigned char>(rd());
    return s;
  }

public:
  Siphash()
  {
    auto seed1 = randomSeed();
    auto seed2 = randomSeed();
    h1_ = boost::hash2::siphash_64(seed1.data(), seed1.size());
    h2_ = boost::hash2::siphash_64(seed2.data(), seed2.size());
  }

  std::size_t hash1(Key const& key) const
  {
    auto h = h1_;
    boost::hash2::hash_append(h, {}, key);
    return boost::hash2::get_integral_result<std::size_t>(h);
  }

  std::size_t hash2(Key const& key) const
  {
    auto h = h2_;
    boost::hash2::hash_append(h, {}, key);
    return boost::hash2::get_integral_result<std::size_t>(h);
  }

  std::size_t operator()(Key const& key) const
  {
    return hash1(key);
  }
};

#endif
