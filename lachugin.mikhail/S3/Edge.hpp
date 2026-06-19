#ifndef EDGE_HPP
#define EDGE_HPP
#include <cstddef>
#include <boost/hash2/hash_append.hpp>
#include <string>
namespace lachugin
{
  struct Edge
  {
    std::string from;
    std::string to;
  };

  struct EdgeEqual
  {
    bool operator()(const Edge& lhs, const Edge& rhs) const;
  };

  struct EdgeHash
  {
    size_t operator()(const Edge& e) const;
  };

  template<class Provider, class Hash, class Flavor>
  void tag_invoke(
  boost::hash2::hash_append_tag,
  Provider,
  Hash& h,
  Flavor const& f,
  Edge const* e)
  {
    boost::hash2::hash_append(h, f, e->from);
    boost::hash2::hash_append(h, f, e->to);
  }
}
#endif

