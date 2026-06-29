#ifndef TABLE_UTILS_HPP
#define TABLE_UTILS_HPP

#include <cstddef>

namespace samarin {

  template< class Table >
  void growIfLoaded(Table& table, const std::size_t extra)
  {
    const std::size_t loadNum = 7;
    const std::size_t loadDen = 10;
    if ((table.size() + extra) * loadDen > table.capacity() * loadNum) {
      table.rehash(table.capacity() * 2);
    }
  }

  template< class Table, class Key, class Value >
  void insertOrGrow(Table& table, const Key& key, const Value& value)
  {
    if (!table.has(key)) {
      growIfLoaded(table, 1);
    }
    table.add(key, value);
  }

  template< class Table, class Key >
  auto subscriptOrGrow(Table& table, const Key& key) -> decltype(table[key])
  {
    if (!table.has(key)) {
      growIfLoaded(table, 1);
    }
    return table[key];
  }

}

#endif
