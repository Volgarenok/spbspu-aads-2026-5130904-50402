#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(QueueMethodsTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  Queue< int > queue;
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  Queue< int > queue;
  queue.push(1);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK(!queue.empty());

  queue.push(2);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_front)
{
  Queue< int > queue;
  queue.push(10);
  BOOST_CHECK_EQUAL(queue.front(), 10);

  queue.push(20);
  BOOST_CHECK_EQUAL(queue.front(), 10);

  queue.front() = 30;
  BOOST_CHECK_EQUAL(queue.front(), 30);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< int > queue;
  queue.push(1);
  queue.push(2);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
