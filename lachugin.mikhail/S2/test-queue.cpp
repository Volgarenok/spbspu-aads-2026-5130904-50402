#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace lachugin;

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  Queue< long long > queue;
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  Queue< long long > queue;
  queue.push(1);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK(!queue.empty());

  queue.push(2);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_front)
{
  Queue< long long > queue;
  queue.push(1);
  BOOST_CHECK_EQUAL(queue.front(), 1);

  queue.push(2);
  BOOST_CHECK_EQUAL(queue.front(), 1);

  queue.front() = 3;
  BOOST_CHECK_EQUAL(queue.front(), 3);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< long long > queue;
  queue.push(1);
  queue.push(2);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
