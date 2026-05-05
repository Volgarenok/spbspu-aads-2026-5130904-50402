#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "ListImpl.hpp"

BOOST_AUTO_TEST_CASE(test_empty)
{
  vishnevskiy::List<int>* t{};
  vishnevskiy::LIter<int> it(t);
  BOOST_TEST(it.curr == nullptr);
}

BOOST_AUTO_TEST_CASE(test_value)
{
  vishnevskiy::List<int>* t = new vishnevskiy::List<int>{5, nullptr};
  vishnevskiy::LIter<int> it(t);
  BOOST_TEST(it.value() == 5);
}

BOOST_AUTO_TEST_CASE(test_hasnext)
{
  vishnevskiy::List<int>* t = new vishnevskiy::List<int>{5, nullptr};
  vishnevskiy::LIter<int> it(t);
  BOOST_TEST(!it.hasNext());
}

BOOST_AUTO_TEST_CASE(test_insert)
{
  vishnevskiy::List<int>* t = new vishnevskiy::List<int>{5, nullptr};
  vishnevskiy::LIter<int> it(t);
  int to_ins = 6;
  it.insert(to_ins);
  ++it;
  BOOST_TEST(it.value() == 6);
}

BOOST_AUTO_TEST_CASE(test_end)
{
  vishnevskiy::List<int>* t = new vishnevskiy::List<int>{5, nullptr};
  vishnevskiy::LIter<int> it(t);
  int to_ins = 6, to_ins2 = 9, to_ins3 = 12;
  it.insert(to_ins);
  it.insert(to_ins2);
  it.insert(to_ins3);
  it.end();
  BOOST_TEST(it.value() == 6);
}
