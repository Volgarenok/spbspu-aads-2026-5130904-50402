#include "array.hpp"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
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

  template< class F >
  bool throws(F f)
  {
    try
    {
      f();
    }
    catch (const std::exception&)
    {
      return true;
    }
    return false;
  }
}

int main()
{
  {
    matveev::Array< int > a;
    check(a.size() == 0 && a.empty(), "default: empty");
  }

  {
    matveev::Array< int > a(5);
    check(a.size() == 5, "sized: size set");
    bool zeroed = true;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
      if (a[i] != 0)
      {
        zeroed = false;
      }
    }
    check(zeroed, "sized: value-initialized to zero");
  }

  {
    matveev::Array< int > a(3, 7);
    check(a.size() == 3 && a[0] == 7 && a[1] == 7 && a[2] == 7, "fill: all equal to value");
    a[1] = 42;
    check(a[1] == 42 && a[0] == 7, "operator[]: write affects only that element");
  }

  {
    matveev::Array< int > a;
    for (int i = 0; i < 100; ++i)
    {
      a.pushBack(i * 2);
    }
    check(a.size() == 100, "pushBack: size grows");
    check(a.capacity() >= 100, "pushBack: capacity covers size");
    bool ok = true;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
      if (a[i] != static_cast< int >(i) * 2)
      {
        ok = false;
      }
    }
    check(ok, "pushBack: elements preserved across growth");
  }

  {
    matveev::Array< int > a(2, 1);
    a.resize(5, 9);
    check(a.size() == 5 && a[0] == 1 && a[1] == 1 && a[2] == 9 && a[4] == 9, "resize: grows and fills new slots");
    a.resize(1, 0);
    check(a.size() == 1 && a[0] == 1, "resize: shrinks, keeps prefix");
  }

  {
    matveev::Array< int > a(3, 5);
    a.clear();
    check(a.empty() && a.size() == 0, "clear: becomes empty");
  }

  {
    matveev::Array< int > a(3, 0);
    check(throws([&]{ a.at(3); }), "at: out of range throws");
    a.at(2) = 11;
    check(a.at(2) == 11, "at: in range works");
  }

  {
    matveev::Array< int > a(3, 4);
    matveev::Array< int > b(a);
    b[0] = 99;
    check(a[0] == 4 && b[0] == 99, "copy: independent storage");
    check(a.size() == 3 && b.size() == 3, "copy: size preserved");
  }

  {
    matveev::Array< int > a(3, 8);
    matveev::Array< int > b(std::move(a));
    check(b.size() == 3 && b[0] == 8, "move: target holds the data");
    check(a.size() == 0 && a.empty(), "move: source emptied");
  }

  {
    matveev::Array< int > a(2, 1);
    matveev::Array< int > b(3, 2);
    a.swap(b);
    check(a.size() == 3 && a[0] == 2 && b.size() == 2 && b[0] == 1, "swap: contents exchanged");
  }

  {
    matveev::Array< bool > flags(4, false);
    flags[2] = true;
    check(!flags[0] && flags[2] && flags.size() == 4, "bool: real bool storage, no proxy surprises");
  }

  {
    matveev::Array< unsigned long long > dp(3, 1000ull);
    dp[1] = 5ull;
    check(dp[0] == 1000ull && dp[1] == 5ull, "ull: usable as algorithm scratch");
  }

  std::cout << "\nALL " << passed << " CHECKS PASSED\n";
  return 0;
}
