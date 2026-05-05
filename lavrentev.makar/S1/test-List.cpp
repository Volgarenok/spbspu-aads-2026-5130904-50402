#define BOOST_TEST_MODULE ListTest
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "List.hpp"

BOOST_AUTO_TEST_CASE(clear_test)
{
  lavrentev::List<size_t> k{};
  lavrentev::LIter<size_t> it;
  it = k.insert(it, 3);
  it = k.insert(it, 4);
  k.clear();
  BOOST_CHECK(k.cbegin() == lavrentev::LCIter<size_t>());
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  lavrentev::List<size_t> k{};
  lavrentev::LIter<size_t> it;
  it = k.insert(it, 1);
  BOOST_TEST(k.front() == 1);
}

BOOST_AUTO_TEST_CASE(popFront_test)
{
  lavrentev::List<size_t> k{};
  BOOST_CHECK(k.popFront() == lavrentev::LIter<size_t>{});
  lavrentev::LIter<size_t> it;
  it = k.insert(it, 1);
  it = k.insert(it, 2);
  BOOST_CHECK(k.popFront() == it);
  BOOST_CHECK(k.popFront() == lavrentev::LIter<size_t>{});
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  lavrentev::List<size_t> k{};
  BOOST_CHECK(k.begin() == lavrentev::LIter<size_t>());

  lavrentev::LIter<size_t> it;
  it = k.insert(it, 3);
  BOOST_CHECK(k.begin() == it);
  BOOST_TEST(*k.begin() == 3);
}

BOOST_AUTO_TEST_CASE(cbegin_test)
{
  lavrentev::List<size_t> k{};
  BOOST_CHECK(k.cbegin() == lavrentev::LCIter<size_t>());

  lavrentev::LIter<size_t> it;
  it = k.insert(it, 3);
  lavrentev::LCIter<size_t> cit = k.cbegin();
  BOOST_CHECK(cit != lavrentev::LCIter<size_t>());
  BOOST_TEST(*cit == 3);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  lavrentev::List<size_t> k{};
  BOOST_CHECK(k.end() == lavrentev::LIter<size_t>());
}

BOOST_AUTO_TEST_CASE(cend_test)
{
  lavrentev::List<size_t> k{};
  BOOST_CHECK(k.cend() == lavrentev::LCIter<size_t>());
}

BOOST_AUTO_TEST_CASE(front_test)
{
  lavrentev::List<size_t> k{};
  BOOST_CHECK_THROW(k.front(), std::out_of_range);

  lavrentev::LIter<size_t> it;
  it = k.insert(it, 3);
  BOOST_TEST(k.front() == 3);

  const lavrentev::List<size_t> &ck = k;
  BOOST_TEST(ck.front() == 3);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  lavrentev::List<size_t> k{};
  BOOST_TEST(k.empty());

  lavrentev::LIter<size_t> it;
  it = k.insert(it, 3);
  BOOST_TEST(!k.empty());
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  lavrentev::List<size_t> k{};
  BOOST_CHECK(k.remove(1) == lavrentev::LIter<size_t>{});
  lavrentev::LIter<size_t> it;
  it = k.insert(it, 1);
  it = k.insert(it, 2);
  BOOST_CHECK(k.remove(1) == it);
  BOOST_CHECK(k.remove(2) == lavrentev::LIter<size_t>{});
}
