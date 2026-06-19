#include "sorts.hpp"
namespace lachugin
{
  void sortEdgesByTo(List< Edge >& edges)
  {
    for (auto it1 = edges.begin(); it1 != edges.end(); ++it1)
    {
      auto minIt = it1;
      for (auto it2 = it1; it2 != edges.end(); ++it2)
      {
        if ((*it2).to < (*minIt).to)
        {
          minIt = it2;
        }
      }
      if (minIt != it1)
      {
        Edge tmp = *it1;
        *it1 = *minIt;
        *minIt = tmp;
      }
    }
  }

  void sortEdgesByFrom(List< Edge >& edges)
  {
    for (auto it1 = edges.begin(); it1 != edges.end(); ++it1)
    {
      auto minIt = it1;
      for (auto it2 = it1; it2 != edges.end(); ++it2)
      {
        if ((*it2).from < (*minIt).from)
        {
          minIt = it2;
        }
      }

      if (minIt != it1)
      {
        Edge tmp = *it1;
        *it1 = *minIt;
        *minIt = tmp;
      }
    }
  }
}

