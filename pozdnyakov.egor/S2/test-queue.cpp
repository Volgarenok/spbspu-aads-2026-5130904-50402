#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace pozdnyakov;

struct EmplaceTarget
{
  int x;
  int y;

  EmplaceTarget(int a, int b):
    x(a),
    y(b)
  {}
};

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(testFifoBehavior)
{
  Queue< int > queue{};
  BOOST_CHECK(queue.empty());

  queue.push(10);
  queue.push(20);
  queue.push(30);

  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.front(), 10);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 20);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 30);

  queue.pop();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(testCompilerGeneratedCopying)
{
  Queue< int > original{};
  original.push(1);
  original.push(2);

  Queue< int > copied = original;
  BOOST_CHECK_EQUAL(copied.front(), 1);

  copied.push(3);

  copied.pop();
  copied.pop();
  BOOST_CHECK_EQUAL(copied.front(), 3);
}

BOOST_AUTO_TEST_CASE(testEmplacePerfectForwarding)
{
  Queue< EmplaceTarget > queue{};

  queue.emplace(10, 20);
  queue.emplace(30, 40);

  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.front().x, 10);
  BOOST_CHECK_EQUAL(queue.front().y, 20);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front().x, 30);
  BOOST_CHECK_EQUAL(queue.front().y, 40);
}

BOOST_AUTO_TEST_SUITE_END()
