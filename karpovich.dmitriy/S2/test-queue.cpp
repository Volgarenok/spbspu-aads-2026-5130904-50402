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

BOOST_AUTO_TEST_CASE(test_move_push)
{
  Queue< std::string > queue;
  std::string str = "hello";

  queue.push(std::move(str));

  BOOST_CHECK_EQUAL(queue.front(), "hello");
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

BOOST_AUTO_TEST_CASE(test_emplace)
{
  Queue< std::pair< int, std::string > > queue;

  queue.emplace(20, "queue");

  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.front().first, 20);
  BOOST_CHECK_EQUAL(queue.front().second, "queue");
}

BOOST_AUTO_TEST_CASE(test_emplace_multiple)
{
  Queue< int > queue;

  queue.emplace(1);
  queue.emplace(2);
  queue.emplace(3);

  BOOST_CHECK_EQUAL(queue.size(), 3);
  BOOST_CHECK_EQUAL(queue.front(), 1);
}

BOOST_AUTO_TEST_CASE(test_emplace_move)
{
  Queue< std::string > queue;

  std::string str = "move_queue";

  queue.emplace(std::move(str));

  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.front(), "move_queue");
}

BOOST_AUTO_TEST_CASE(test_emplace_empty)
{
  Queue< std::string > queue;

  queue.emplace("hello");

  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.front(), "hello");
}

BOOST_AUTO_TEST_SUITE_END()
