#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace novikov;

BOOST_AUTO_TEST_SUITE(QueueTest)

BOOST_AUTO_TEST_CASE(empty)
{
  Queue< int > queue;
  BOOST_CHECK(queue.empty() == true);
  queue.push(10);
  BOOST_CHECK(queue.empty() == false);
}

BOOST_AUTO_TEST_CASE(push_and_drop)
{
  Queue< int > queue;
  queue.push(5);
  queue.push(6);
  queue.push(7);

  BOOST_CHECK(queue.drop() == 5);
  BOOST_CHECK(queue.drop() == 6);
  BOOST_CHECK(queue.drop() == 7);
  BOOST_CHECK(queue.empty() == true);
}

BOOST_AUTO_TEST_CASE(front)
{
  Queue< int > queue;
  queue.push(8);
  queue.push(9);
  queue.push(10);

  BOOST_CHECK(queue.front() == 8);
  queue.drop();
  BOOST_CHECK(queue.front() == 9);
}

BOOST_AUTO_TEST_CASE(empty_exception)
{
  Queue< int > queue;
  BOOST_CHECK_THROW(queue.drop(), std::runtime_error);
  BOOST_CHECK_THROW(queue.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(const_front)
{
  const Queue< int > queue;
  BOOST_CHECK_THROW(queue.front(), std::runtime_error);

  Queue< int > queue2;
  queue2.push(42);
  const Queue< int > &const_queue = queue2;
  BOOST_CHECK(const_queue.front() == 42);
}

BOOST_AUTO_TEST_SUITE_END()
