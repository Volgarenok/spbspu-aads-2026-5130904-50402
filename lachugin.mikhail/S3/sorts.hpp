#ifndef SORTS_HPP
#define SORTS_HPP

#include "GraphStorage.hpp"
namespace lachugin
{
  template< class T >
  void sortList(List< T >& list)
  {
    if (list.size() < 2)
    {
      return;
    }
    for (auto it1 = list.begin(); it1 != list.end(); ++it1)
    {
      auto minIt = it1;
      for (auto it2 = it1; it2 != list.end(); ++it2)
      {
        if (*it2 < *minIt)
        {
          minIt = it2;
        }
      }
      if (minIt != it1)
      {
        T tmp = *it1;
        *it1 = *minIt;
        *minIt = tmp;
      }
    }
  }

  void sortEdgesByTo(List< Edge >& edges);
  void sortEdgesByFrom(List< Edge >& edges);
}

#endif
