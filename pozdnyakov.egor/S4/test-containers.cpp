#include <boost/test/unit_test.hpp>
#include "containers.hpp"

BOOST_AUTO_TEST_SUITE(ContainersTests)

BOOST_AUTO_TEST_CASE(test_map)
{
  pozdnyakov::Map< int, std::string > map;
  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(1, "uno");

  BOOST_CHECK(map.contains(1));
  BOOST_CHECK_EQUAL(map.at(1), "uno");
}

BOOST_AUTO_TEST_CASE(test_set)
{
  pozdnyakov::Set< int > set;
  set.insert(1);
  set.insert(2);
  set.insert(1);

  BOOST_CHECK(set.contains(1));
  BOOST_CHECK(set.contains(2));
  BOOST_CHECK(!set.contains(3));

  set.erase(1);
  BOOST_CHECK(!set.contains(1));
}

BOOST_AUTO_TEST_CASE(test_multimap)
{
  pozdnyakov::MultiMap< int, std::string > mmap;
  mmap.insert(1, "one");
  mmap.insert(2, "two");
  mmap.insert(1, "uno");

  BOOST_CHECK(mmap.contains(1));
  mmap.erase(1);
  BOOST_CHECK(!mmap.contains(1));
  BOOST_CHECK(mmap.contains(2));
}

BOOST_AUTO_TEST_CASE(test_multiset)
{
  pozdnyakov::MultiSet< int > mset;
  mset.insert(1);
  mset.insert(1);
  mset.insert(2);

  BOOST_CHECK(mset.contains(1));
  mset.erase(1);
  BOOST_CHECK(!mset.contains(1));
  BOOST_CHECK(mset.contains(2));
}

BOOST_AUTO_TEST_SUITE_END()
