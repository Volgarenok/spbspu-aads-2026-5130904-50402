#define BOOST_TEST_MODULE ListTest

#include <boost/test/included/unit_test.hpp>
#include "List.hpp"

BOOST_AUTO_TEST_CASE(clear_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LIter<size_t> iter;

  iter = list.insert(iter, 3);
  iter = list.insert(iter, 4);

  list.clear();

  lavrentev::LCIter<size_t> empty_iter;
  BOOST_CHECK(list.cbegin() == empty_iter);
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LIter<size_t> iter;

  iter = list.insert(iter, 1);

  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LIter<size_t> empty_iter;

  BOOST_CHECK(list.popFront() == empty_iter);

  lavrentev::LIter<size_t> iter;
  iter = list.insert(iter, 1);
  iter = list.insert(iter, 2);

  BOOST_CHECK(list.popFront() == iter);
  BOOST_CHECK(list.popFront() == empty_iter);
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LIter<size_t> empty_iter;

  BOOST_CHECK(list.begin() == empty_iter);

  lavrentev::LIter<size_t> iter;
  iter = list.insert(iter, 3);

  BOOST_CHECK(list.begin() == iter);
  BOOST_TEST(*list.begin() == 3);
}

BOOST_AUTO_TEST_CASE(cbegin_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LCIter<size_t> empty_iter;

  BOOST_CHECK(list.cbegin() == empty_iter);

  lavrentev::LIter<size_t> iter;
  iter = list.insert(iter, 3);

  lavrentev::LCIter<size_t> citer = list.cbegin();

  BOOST_CHECK(citer != empty_iter);
  BOOST_TEST(*citer == 3);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LIter<size_t> empty_iter;

  BOOST_CHECK(list.end() == empty_iter);
}

BOOST_AUTO_TEST_CASE(cend_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LCIter<size_t> empty_iter;

  BOOST_CHECK(list.cend() == empty_iter);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  lavrentev::List<size_t> list{};

  BOOST_CHECK_THROW(list.front(), std::out_of_range);

  lavrentev::LIter<size_t> iter;
  iter = list.insert(iter, 3);

  BOOST_TEST(list.front() == 3);

  const lavrentev::List<size_t> &const_list = list;
  BOOST_TEST(const_list.front() == 3);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  lavrentev::List<size_t> list{};

  BOOST_TEST(list.empty());

  lavrentev::LIter<size_t> iter;
  iter = list.insert(iter, 3);

  BOOST_TEST(!list.empty());
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  lavrentev::List<size_t> list{};
  lavrentev::LIter<size_t> empty_iter;

  BOOST_CHECK(list.remove(1) == empty_iter);

  lavrentev::LIter<size_t> iter;
  iter = list.insert(iter, 1);
  iter = list.insert(iter, 2);

  BOOST_CHECK(list.remove(1) == iter);
  BOOST_CHECK(list.remove(2) == empty_iter);
}
