#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(StackMethodsTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  Stack< int > stack;
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  Stack< int > stack;
  stack.push(1);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK(!stack.empty());

  stack.push(2);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_push)
{
  Stack< std::string > stack;
  std::string str = "hello";

  stack.push(std::move(str));

  BOOST_CHECK_EQUAL(stack.top(), "hello");
}

BOOST_AUTO_TEST_CASE(test_top)
{
  Stack< int > stack;
  stack.push(10);
  BOOST_CHECK_EQUAL(stack.top(), 10);

  stack.push(20);
  BOOST_CHECK_EQUAL(stack.top(), 20);

  stack.top() = 30;
  BOOST_CHECK_EQUAL(stack.top(), 30);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Stack< int > stack;
  stack.push(1);
  stack.push(2);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_emplace)
{
  Stack< std::pair< int, std::string > > stack;

  stack.emplace(10, "stack");

  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.top().first, 10);
  BOOST_CHECK_EQUAL(stack.top().second, "stack");
}

BOOST_AUTO_TEST_CASE(test_emplace_multiple)
{
  Stack< int > stack;

  stack.emplace(1);
  stack.emplace(2);
  stack.emplace(3);

  BOOST_CHECK_EQUAL(stack.size(), 3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
}

BOOST_AUTO_TEST_CASE(test_emplace_move)
{
  Stack< std::string > stack;

  std::string str = "move_stack";

  stack.emplace(std::move(str));

  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.top(), "move_stack");
}

BOOST_AUTO_TEST_CASE(test_emplace_empty)
{
  Stack< std::string > stack;

  stack.emplace("hello");

  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.top(), "hello");
}

BOOST_AUTO_TEST_SUITE_END()
