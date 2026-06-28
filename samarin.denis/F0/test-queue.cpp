#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(queue)

BOOST_AUTO_TEST_CASE(fifo_order)
{
  samarin::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  BOOST_TEST(queue.drop() == 1);
  BOOST_TEST(queue.drop() == 2);
  BOOST_TEST(queue.drop() == 3);
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(reuse_after_empty)
{
  samarin::Queue< int > queue;
  queue.push(1);
  BOOST_TEST(queue.drop() == 1);
  queue.push(2);
  queue.push(3);
  BOOST_TEST(queue.drop() == 2);
  BOOST_TEST(queue.drop() == 3);
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  samarin::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  samarin::Queue< int > copy = queue;
  BOOST_TEST(copy.drop() == 1);
  BOOST_TEST(queue.drop() == 1);
}

BOOST_AUTO_TEST_CASE(drop_empty_throws)
{
  samarin::Queue< int > queue;
  BOOST_CHECK_THROW(queue.drop(), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
