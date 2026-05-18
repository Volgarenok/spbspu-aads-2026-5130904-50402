#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace bukreev;

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(test_push)
{
  Queue< int > queue;
  queue.push(1);
  queue.push(2);

  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< int > queue;
  queue.push(1);
  queue.push(2);
  int popped = queue.pop();

  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(popped, 1);
  popped = queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 0);
  BOOST_CHECK_EQUAL(popped, 2);
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()
