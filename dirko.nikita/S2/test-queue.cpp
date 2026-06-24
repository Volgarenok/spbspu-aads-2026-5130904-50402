#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(queueTest)
BOOST_AUTO_TEST_CASE(empty)
{
  Queue< int > q;
  BOOST_CHECK_EQUAL(q.empty(), true);
}

BOOST_AUTO_TEST_CASE(size)
{
  Queue< int > q;
  BOOST_CHECK_EQUAL(q.size(), 0);
  q.push(1);
  BOOST_CHECK_EQUAL(q.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_push_get)
{
  Queue< int > q;
  q.push(1);
  BOOST_CHECK_EQUAL(q.get(), 1);
}
BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.pop();
  BOOST_CHECK_EQUAL(q.get(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_push)
{
  Queue< std::string > queue;
  std::string str = "faxryzen";

  queue.push(std::move(str));

  BOOST_CHECK_EQUAL(queue.get(), "faxryzen");
}
BOOST_AUTO_TEST_CASE(test_emplace)
{
  Queue< std::pair< int, std::string > > queue;

  queue.emplace(20, "queue");

  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.get().first, 20);
  BOOST_CHECK_EQUAL(queue.get().second, "queue");
}

BOOST_AUTO_TEST_CASE(test_emplace_multiple)
{
  Queue< int > queue;

  queue.emplace(1);
  queue.emplace(2);
  queue.emplace(3);

  BOOST_CHECK_EQUAL(queue.size(), 3);
  BOOST_CHECK_EQUAL(queue.get(), 1);
}

BOOST_AUTO_TEST_CASE(test_emplace_move)
{
  Queue< std::string > queue;

  std::string str = "move_queue";

  queue.emplace(std::move(str));

  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.get(), "move_queue");
}

BOOST_AUTO_TEST_CASE(test_emplace_empty)
{
  Queue< std::string > queue;

  queue.emplace("hello");

  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.get(), "hello");
}
BOOST_AUTO_TEST_SUITE_END()
