#include <utility>

namespace karpovich
{
  namespace details
  {
    template< class Key, class Value >
    struct CuckooEntry
    {
      std::pair< Key, Value > data_;
      bool occupied_;
      CuckooEntry();
      CuckooEntry(const Key &k, const Value &v);
    };
    template< class Key, class Value >
    CuckooEntry< Key, Value >::CuckooEntry():
      data_(),
      occupied_(false)
    {}
    template< class Key, class Value >
    CuckooEntry< Key, Value >::CuckooEntry(const Key &k, const Value &v):
      data_(k, v),
      occupied_(true)
    {}
  }
}
