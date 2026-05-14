#include <array>
#include <cstddef>
#include <random>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

template <class Key>
class Siphash
{
  boost::hash2::siphash_64 h_;
  static std::array<unsigned char, 16> make_seed_16()
  {
    std::array<unsigned char, 16> s{};
    std::random_device rd;
    for (auto &b : s)
    {
      b = static_cast<unsigned char>(rd());
    }

    return s;
  }

public:
  Siphash()
  {
    auto seed = make_seed_16();
    h_ = boost::hash2::siphash_64(seed.data(), seed.size());
  }

  explicit Siphash(std::array<unsigned char, 16> const &seed) : h_(seed.data(), seed.size()) {}
  Siphash(unsigned char const *p, std::size_t n) : h_(p, n) {}

  std::size_t operator()(Key const &key) const
  {
    auto h = h_;
    boost::hash2::hash_append(h, {}, key);
    return boost::hash2::get_integral_result<std::size_t>(h);
  }

  std::size_t operator()(const std::pair<std::string, std::string>& p) const
  {
    Siphash<std::string> h;

    std::size_t h1 = h(p.first);
    std::size_t h2 = h(p.second);

    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
  }
};

template <>
class Siphash<std::pair<std::string, std::string>>
{
  Siphash<std::string> stringHasher_;

public:
  Siphash() = default;

  std::size_t operator()(const std::pair<std::string, std::string>& p) const
  {
    std::size_t h1 = stringHasher_(p.first);
    std::size_t h2 = stringHasher_(p.second);
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
  }
};
