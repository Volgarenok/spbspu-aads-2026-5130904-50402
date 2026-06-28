#include <boost/test/unit_test.hpp>
#include <string>
#include <list.hpp>
#include "list_utils.hpp"

using List = samarin::List< int >;

BOOST_AUTO_TEST_SUITE(list)

BOOST_AUTO_TEST_CASE(push_front_order)
{
  List list;
  list.push_front(1);
  list.push_front(2);
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(samarin::listSize(list) == 2u);
}

BOOST_AUTO_TEST_CASE(insert_after_appends)
{
  List list;
  samarin::LIter< int > tail = list.before_begin();
  tail = list.insert_after(tail, 1);
  tail = list.insert_after(tail, 2);
  tail = list.insert_after(tail, 3);
  BOOST_TEST(samarin::listAt(list, 0) == 1);
  BOOST_TEST(samarin::listAt(list, 2) == 3);
}

BOOST_AUTO_TEST_CASE(erase_after_removes)
{
  List list;
  samarin::LIter< int > tail = list.before_begin();
  tail = list.insert_after(tail, 1);
  list.insert_after(tail, 2);
  list.erase_after(list.before_begin());
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(samarin::listSize(list) == 1u);
}

BOOST_AUTO_TEST_CASE(copy_is_deep)
{
  List list;
  list.push_front(5);
  List copy = list;
  copy.front() = 9;
  BOOST_TEST(list.front() == 5);
  BOOST_TEST(copy.front() == 9);
}

BOOST_AUTO_TEST_CASE(move_transfers)
{
  List list;
  list.push_front(7);
  List moved = std::move(list);
  BOOST_TEST(moved.front() == 7);
}

BOOST_AUTO_TEST_CASE(out_of_range_throws)
{
  List list;
  BOOST_CHECK_THROW(samarin::listAt(list, 0), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
