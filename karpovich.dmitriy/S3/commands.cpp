#include "commands.hpp"
#include "../common/Vector.hpp"

namespace details
{

  void sortStrings(karpovich::Vector< std::string > &vec)
  {
    for (size_t i = 1; i < vec.getSize(); ++i) {
      std::string key = vec[i];
      size_t j = i;
      while (j > 0 && vec[j - 1] > key) {
        vec[j] = vec[j - 1];
        --j;
      }
      vec[j] = key;
    }
  }

  void sortWeights(karpovich::Vector< size_t > &vec)
  {
    for (size_t i = 1; i < vec.getSize(); ++i) {
      size_t key = vec[i];
      size_t j = i;
      while (j > 0 && vec[j - 1] > key) {
        vec[j] = vec[j - 1];
        --j;
      }
      vec[j] = key;
    }
  }

}

void karpovich::cmdGraphs(std::istream &, std::ostream &out, karpovich::GraphSet &graphs)
{
  karpovich::Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    names.pushBack((*it).first);
  }
  details::sortStrings(names);
  for (size_t i = 0; i < names.getSize(); ++i) {
    out << names[i] << "\n";
  }
}

void karpovich::cmdVertexes(std::istream &in, std::ostream &out, karpovich::GraphSet &graphs)
{
  std::string g_name;
  in >> g_name;
  karpovich::Graph &g = graphs.get(g_name);
  karpovich::Vector< std::string > verts;
  for (auto it = g.vertices.begin(); it != g.vertices.end(); ++it) {
    verts.pushBack(*it);
  }
  details::sortStrings(verts);
  for (size_t i = 0; i < verts.getSize(); ++i) {
    out << verts[i] << "\n";
  }
}
