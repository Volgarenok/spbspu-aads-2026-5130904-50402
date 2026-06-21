#include "Edge.hpp"
#include <boost/hash2/sha1.hpp>
#include <string>
#include <cstring>
#include <algorithm>
namespace lachugin
{
  bool EdgeEqual::operator()(const Edge &lhs, const Edge &rhs) const
  {
    return lhs.from == rhs.from && lhs.to == rhs.to;
  }

  size_t EdgeHash::operator()(const Edge& e) const
  {
    boost::hash2::sha1_160 h;
    boost::hash2::hash_append(h, boost::hash2::default_flavor(), e);
    auto digest = h.result();
    size_t value = 0;
    std::memcpy(&value, digest.data(), std::min(sizeof(size_t), digest.size()));
    return value;
  }

}
