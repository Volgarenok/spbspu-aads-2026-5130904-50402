#ifndef ITEM_HPP
#define ITEM_HPP
#include <algorithm>

namespace lachugin
{
  template< class Key, class Value >
  struct CuckooItem
  {
    std::pair< Key, Value > data;
    bool occupied;
    CuckooItem():
        occupied(false)
    {}
  };

}


#endif
