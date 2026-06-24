#define BOOST_TEST_MODULE ListTest
#include <boost/test/included/unit_test.hpp>
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

BOOST_AUTO_TEST_CASE(splice_test)
{
  lavrentev::List<size_t> k1{};
  k1.insert(k1.end(), 1);
  k1.insert(k1.end(), 2);

  lavrentev::List<size_t> k2{};
  lavrentev::LIter<size_t> target = k2.insert(k2.end(), 3);
  k2.insert(k2.end(), 4);

  k1.splice(k1.end(), k2, target);
  
  lavrentev::LIter<size_t> res = k1.begin();
  BOOST_TEST(*res == 1); ++res; 
  BOOST_TEST(*res == 2); ++res; 
  BOOST_TEST(*res == 3);

  k1.splice(k1.end(), k2);
  BOOST_CHECK(k2.empty());

  res = k1.begin();
  ++res;
  ++res;
  ++res;
  BOOST_TEST(*res == 4);
}

BOOST_AUTO_TEST_CASE(merge_test)
{
  lavrentev::List<size_t> k1{};
  lavrentev::LIter<size_t> it1;
  it1 = k1.insert(it1, 1);
  it1 = k1.insert(it1, 3);

  lavrentev::List<size_t> k2{};
  lavrentev::LIter<size_t> it2;
  it2 = k2.insert(it2, 2);
  it2 = k2.insert(it2, 4);

  k1.merge(k2);

  lavrentev::LIter<size_t> res = k1.begin();
  BOOST_TEST(*res == 1);
  ++res;
  BOOST_TEST(*res == 2);
  ++res;
  BOOST_TEST(*res == 3);
  ++res;
  BOOST_TEST(*res == 4);
  BOOST_TEST(k2.empty());
}

BOOST_AUTO_TEST_CASE(sort_test)
{
  lavrentev::List<size_t> k{};
  lavrentev::LIter<size_t> it;
  it = k.insert(it, 4);
  it = k.insert(it, 2);
  it = k.insert(it, 3);
  it = k.insert(it, 1);

  k.sort();

  lavrentev::LIter<size_t> res = k.begin();
  BOOST_TEST(*res == 1);
  ++res;
  BOOST_TEST(*res == 2);
  ++res;
  BOOST_TEST(*res == 3);
  ++res;
  BOOST_TEST(*res == 4);
}

BOOST_AUTO_TEST_CASE(partition_test)
{
  lavrentev::List<size_t> k{};
  lavrentev::LIter<size_t> it;
  it = k.insert(it, 1);
  it = k.insert(it, 2);
  it = k.insert(it, 3);
  it = k.insert(it, 4);

  struct IsEven
  {
    bool operator()(size_t n) const { return n % 2 == 0; }
  };

  k.partition(IsEven());

  lavrentev::LIter<size_t> res = k.begin();
  BOOST_TEST(*res == 2);
  ++res;
  BOOST_TEST(*res == 4);
  ++res;
  BOOST_TEST(*res == 1);
  ++res;
  BOOST_TEST(*res == 3);
}
