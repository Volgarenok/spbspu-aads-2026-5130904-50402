#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(queue_tests)

BOOST_AUTO_TEST_CASE(push_and_drop_fifo)
{
  samarin::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_TEST(q.drop() == 1);
  BOOST_TEST(q.drop() == 2);
  BOOST_TEST(q.drop() == 3);
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(drop_empty_throws)
{
  samarin::Queue< int > q;
  BOOST_CHECK_THROW(q.drop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(reuse_after_becoming_empty)
{
  samarin::Queue< int > q;
  q.push(1);
  q.drop();
  q.push(2);
  q.push(3);
  BOOST_TEST(q.drop() == 2);
  BOOST_TEST(q.drop() == 3);
}

BOOST_AUTO_TEST_CASE(copy_keeps_independent_tail)
{
  samarin::Queue< int > q;
  q.push(1);
  q.push(2);
  samarin::Queue< int > copy(q);
  copy.push(3);
  q.push(99);
  BOOST_TEST(copy.drop() == 1);
  BOOST_TEST(copy.drop() == 2);
  BOOST_TEST(copy.drop() == 3);
  BOOST_TEST(q.drop() == 1);
  BOOST_TEST(q.drop() == 2);
  BOOST_TEST(q.drop() == 99);
}

BOOST_AUTO_TEST_SUITE_END()
