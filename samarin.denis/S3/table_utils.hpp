#ifndef TABLE_UTILS_HPP
#define TABLE_UTILS_HPP

#include <cstddef>
#include <stdexcept>

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
    while (true) {
      try {
        table.add(key, value);
        return;
      } catch (const std::length_error&) {
        table.rehash(table.capacity() * 2);
      }
    }
  }

  template< class Table, class Key >
  auto subscriptOrGrow(Table& table, const Key& key) -> decltype(table[key])
  {
    if (!table.has(key)) {
      growIfLoaded(table, 1);
    }
    while (true) {
      try {
        return table[key];
      } catch (const std::length_error&) {
        table.rehash(table.capacity() * 2);
      }
    }
  }

}

#endif
