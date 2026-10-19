#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace permyakov;

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(test_push)
{
  Queue< int > que;
  que.push(1);
  BOOST_CHECK_EQUAL(que.front(), 1);
  que.push(2);
  BOOST_CHECK_EQUAL(que.front(), 1);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< int > que;
  que.push(1);
  que.pop();
  BOOST_CHECK_EQUAL(que.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_front)
{
  Queue< int > que;
  que.push(1);
  que.push(2);
  que.push(3);
  BOOST_CHECK_EQUAL(que.front(), 1);
  que.pop();
  BOOST_CHECK_EQUAL(que.front(), 2);
  que.pop();
  BOOST_CHECK_EQUAL(que.front(), 3);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  Queue< int > que;
  BOOST_CHECK_EQUAL(que.size(), 0);
  que.push(1);
  BOOST_CHECK_EQUAL(que.size(), 1);
  que.push(2);
  BOOST_CHECK_EQUAL(que.size(), 2);
  que.pop();
  BOOST_CHECK_EQUAL(que.size(), 1);
  que.pop();
  BOOST_CHECK_EQUAL(que.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_is_empty)
{
  Queue< int > que;
  BOOST_CHECK(que.isEmpty());
  que.push(1);
  que.pop();
  BOOST_CHECK(que.isEmpty());
}

BOOST_AUTO_TEST_SUITE_END()
