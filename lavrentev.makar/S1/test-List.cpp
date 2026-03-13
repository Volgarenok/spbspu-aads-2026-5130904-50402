#define BOOST_TEST_MODULE ListTest
#include <boost/test/included/unit_test.hpp>
//#include <sstream>
#include "List.hpp"

BOOST_AUTO_TEST_CASE(clear_test)
{
  lavrentev::List<int> k{};
  lavrentev::LIter<int> it;
  it = k.insert(it, 3);
  it = k.insert(it, 4);
  k.clear();
  BOOST_CHECK(k.cbegin() == lavrentev::LCIter<int>());
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  lavrentev::List<int> k{};
  lavrentev::LIter<int> it;
  it = k.insert(it, 1);
  BOOST_TEST(k.front() == 1);
}

BOOST_AUTO_TEST_CASE(popFront_test)
{
  lavrentev::List<int> k{};
  BOOST_CHECK(k.popFront() == lavrentev::LIter<int>{});
  lavrentev::LIter<int> it;
  it = k.insert(it, 1);
  it = k.insert(it, 2);
  BOOST_CHECK(k.popFront() == it);
  BOOST_CHECK(k.popFront() == lavrentev::LIter<int>{});
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  lavrentev::List<int> k{};
  BOOST_CHECK(k.begin() == lavrentev::LIter<int>());
  
  lavrentev::LIter<int> it;
  it = k.insert(it, 3);
  BOOST_CHECK(k.begin() == it);
  BOOST_TEST(*k.begin() == 3);
}

BOOST_AUTO_TEST_CASE(cbegin_test)
{
  lavrentev::List<int> k{};
  BOOST_CHECK(k.cbegin() == lavrentev::LCIter<int>());
  
  lavrentev::LIter<int> it;
  it = k.insert(it, 3);
  lavrentev::LCIter<int> cit = k.cbegin();
  BOOST_CHECK(cit != lavrentev::LCIter<int>());
  BOOST_TEST(*cit == 3);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  lavrentev::List<int> k{};
  BOOST_CHECK_THROW(k.front(), std::out_of_range);
  
  lavrentev::LIter<int> it;
  it = k.insert(it, 3);
  BOOST_TEST(k.front() == 3);

  const lavrentev::List<int>& ck = k;
  BOOST_TEST(ck.front() == 3);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  lavrentev::List<int> k{};
  BOOST_TEST(k.empty());
  
  lavrentev::LIter<int> it;
  it = k.insert(it, 3);
  BOOST_TEST(!k.empty());
}

BOOST_AUTO_TEST_CASE(hasNext_test)
{
  lavrentev::List<int> k{};
  lavrentev::LCIter<int> it{};
  BOOST_TEST(!it.hasNext());

  it = k.insert(it, 3);
  BOOST_TEST(!it.hasNext());

  k.insert(it, 4);
  BOOST_TEST(it.hasNext());
}
