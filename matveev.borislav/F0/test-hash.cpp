#include "hash-table.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
  struct FnvHash
  {
    std::size_t operator()(const std::string& value) const
    {
      std::uint64_t hash = 1469598103934665603ull;

      for (std::size_t i = 0; i < value.size(); ++i)
      {
        hash ^= static_cast< std::uint64_t >(static_cast< unsigned char >(value[i]));
        hash *= 1099511628211ull;
      }

      return static_cast< std::size_t >(hash);
    }
  };

  struct ZeroHash
  {
    std::size_t operator()(const std::string&) const
    {
      return 0;
    }
  };

  struct StrEqual
  {
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
      return lhs == rhs;
    }
  };

  using Table = matveev::HashTable< std::string, int, FnvHash, StrEqual >;
  using CollidingTable = matveev::HashTable< std::string, int, ZeroHash, StrEqual >;

  int passed = 0;

  void check(bool condition, const char* name)
  {
    if (!condition)
    {
      std::cout << "FAIL: " << name << '\n';
      std::exit(1);
    }

    ++passed;
    std::cout << "ok: " << name << '\n';
  }
}

int main()
{
  {
    Table t;
    t.add("a", 1);
    t.add("b", 2);
    t.add("c", 3);

    check(t.size() == 3, "size after 3 adds");
    check(t.has("a") && t.has("b") && t.has("c"), "has finds all");
    check(!t.has("z"), "has rejects missing");
    check(t.at("a") == 1 && t.at("b") == 2 && t.at("c") == 3, "at returns values");
  }

  {
    Table t;
    t.add("x", 10);
    t.at("x") = 99;
    check(t.at("x") == 99, "at allows mutation");
  }

  {
    Table t;
    t.add("a", 1);

    bool threw = false;
    try
    {
      t.add("a", 2);
    }
    catch (const std::logic_error&)
    {
      threw = true;
    }
    check(threw, "duplicate add throws logic_error");
    check(t.at("a") == 1, "duplicate add left value untouched");

    threw = false;
    try
    {
      t.at("missing");
    }
    catch (const std::out_of_range&)
    {
      threw = true;
    }
    check(threw, "at missing throws out_of_range");
  }

  {
    Table t;
    t.add("a", 1);
    t.add("b", 2);

    int dropped = t.drop("a");
    check(dropped == 1, "drop returns value");
    check(!t.has("a"), "drop removes key");
    check(t.has("b"), "drop keeps other keys");
    check(t.size() == 1, "size after drop");

    bool threw = false;
    try
    {
      t.drop("a");
    }
    catch (const std::out_of_range&)
    {
      threw = true;
    }
    check(threw, "drop missing throws out_of_range");
  }

  {
    CollidingTable t(64);
    const int count = 20;

    for (int i = 0; i < count; ++i)
    {
      t.add("key" + std::to_string(i), i);
    }

    check(t.size() == static_cast< std::size_t >(count), "colliding: size correct");
    check(t.maxProbeLength() == static_cast< std::size_t >(count - 1), "colliding: max psl == N-1");

    bool all_found = true;
    for (int i = 0; i < count; ++i)
    {
      if (!t.has("key" + std::to_string(i)) || t.at("key" + std::to_string(i)) != i)
      {
        all_found = false;
      }
    }
    check(all_found, "colliding: all keys findable in one chain");
  }

  {
    CollidingTable t(64);
    const int count = 30;

    for (int i = 0; i < count; ++i)
    {
      t.add("k" + std::to_string(i), i);
    }

    for (int i = 0; i < count; i += 3)
    {
      int dropped = t.drop("k" + std::to_string(i));
      check(dropped == i, "backward-shift: drop returns right value");
    }

    bool survivors_ok = true;
    int survivors = 0;
    for (int i = 0; i < count; ++i)
    {
      const bool should_exist = (i % 3 != 0);

      if (t.has("k" + std::to_string(i)) != should_exist)
      {
        survivors_ok = false;
      }

      if (should_exist)
      {
        ++survivors;
        if (t.at("k" + std::to_string(i)) != i)
        {
          survivors_ok = false;
        }
      }
    }
    check(survivors_ok, "backward-shift: survivors intact, deleted gone");
    check(t.size() == static_cast< std::size_t >(survivors), "backward-shift: size matches survivors");

    t.add("k0", 1000);
    check(t.at("k0") == 1000, "backward-shift: reinsert after delete");
  }

  {
    Table t(4);
    const int count = 5000;

    for (int i = 0; i < count; ++i)
    {
      t.add("item-" + std::to_string(i), i * 2);
    }

    check(t.size() == static_cast< std::size_t >(count), "growth: size correct");
    check(t.loadFactor() <= t.maxLoadFactor(), "growth: load factor within max");

    bool all_ok = true;
    for (int i = 0; i < count; ++i)
    {
      if (t.at("item-" + std::to_string(i)) != i * 2)
      {
        all_ok = false;
      }
    }
    check(all_ok, "growth: all values correct after many rehashes");
    check(t.maxProbeLength() < 50, "growth: probe length stays small with good hash");
  }

  {
    Table t;
    std::vector< std::string > keys = { "alpha", "beta", "gamma", "delta", "epsilon" };

    for (std::size_t i = 0; i < keys.size(); ++i)
    {
      t.add(keys[i], static_cast< int >(i));
    }

    std::size_t seen = 0;
    long sum = 0;
    for (Table::const_iterator it = t.cbegin(); it != t.cend(); ++it)
    {
      ++seen;
      sum += it->value;
    }
    check(seen == keys.size(), "iterator: visits every occupied slot");
    check(sum == 0 + 1 + 2 + 3 + 4, "iterator: values summed correctly");

    long mutated = 0;
    for (Table::iterator it = t.begin(); it != t.end(); ++it)
    {
      it->value += 10;
      mutated += it->value;
    }
    check(mutated == (10 + 11 + 12 + 13 + 14), "iterator: non-const iteration mutates values");
  }

  {
    Table a;
    a.add("one", 1);
    a.add("two", 2);

    Table b(a);
    b.add("three", 3);
    b.at("one") = 111;

    check(a.size() == 2, "copy: original size unchanged");
    check(a.at("one") == 1, "copy: original value unchanged");
    check(!a.has("three"), "copy: insert into copy does not touch original");
    check(b.size() == 3 && b.at("one") == 111, "copy: copy mutated independently");
  }

  {
    Table t;
    t.add("a", 1);
    t.add("b", 2);
    t.clear();
    check(t.empty() && t.size() == 0, "clear: empties table");
    check(!t.has("a"), "clear: keys gone");
    t.add("a", 5);
    check(t.at("a") == 5, "clear: reusable after clear");
  }

  {
    Table t;
    const int count = 10000;

    for (int i = 0; i < count; ++i)
    {
      t.add("s" + std::to_string(i), i);
    }
    for (int i = 0; i < count; i += 2)
    {
      t.drop("s" + std::to_string(i));
    }

    bool ok = true;
    for (int i = 0; i < count; ++i)
    {
      const bool should_exist = (i % 2 == 1);
      if (t.has("s" + std::to_string(i)) != should_exist)
      {
        ok = false;
      }
    }
    check(ok, "stress: 10k insert + 5k delete consistent");
    check(t.size() == static_cast< std::size_t >(count / 2), "stress: final size correct");
  }

  std::cout << "\nALL " << passed << " CHECKS PASSED\n";
  return 0;
}
