#include <boost/test/unit_test.hpp>
#include "stack.hpp"

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

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(testLifoBehavior)
{
  Stack< int > stack{};
  BOOST_CHECK(stack.empty());

  stack.push(10);
  stack.push(20);
  stack.push(30);

  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.top(), 30);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 20);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 10);

  stack.pop();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(testClearMethod)
{
  Stack< int > stack{};
  stack.push(1);
  stack.push(2);

  stack.clear();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(testEmplacePerfectForwarding)
{
  Stack< EmplaceTarget > stack{};

  stack.emplace(10, 20);

  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.top().x, 10);
  BOOST_CHECK_EQUAL(stack.top().y, 20);

  stack.emplace(99, 100);
  BOOST_CHECK_EQUAL(stack.top().x, 99);
  BOOST_CHECK_EQUAL(stack.top().y, 100);
}

BOOST_AUTO_TEST_SUITE_END()
