#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(stackTest)
BOOST_AUTO_TEST_CASE(empty)
{
  Stack< int > st;
  BOOST_CHECK_EQUAL(st.empty(), true);
}

BOOST_AUTO_TEST_CASE(size)
{
  Stack< int > st;
  BOOST_CHECK_EQUAL(st.size(), 0);
  st.push(1);
  BOOST_CHECK_EQUAL(st.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_push_get)
{
  Stack< int > st;
  st.push(1);
  BOOST_CHECK_EQUAL(st.get(), 1);
}
BOOST_AUTO_TEST_CASE(test_pop)
{
  Stack< int > st;
  st.push(1);
  st.push(2);
  st.pop();
  BOOST_CHECK_EQUAL(st.get(), 1);
}
BOOST_AUTO_TEST_CASE(test_move_push)
{
  Stack< std::string > queue;
  std::string str = "faxryzen";

  queue.push(std::move(str));

  BOOST_CHECK_EQUAL(queue.get(), "faxryzen");
}
BOOST_AUTO_TEST_CASE(test_emplace)
{
  Stack< std::pair< int, std::string > > stack;

  stack.emplace(10, "stack");

  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.get().first, 10);
  BOOST_CHECK_EQUAL(stack.get().second, "stack");
}

BOOST_AUTO_TEST_CASE(test_emplace_multiple)
{
  Stack< int > stack;

  stack.emplace(1);
  stack.emplace(2);
  stack.emplace(3);

  BOOST_CHECK_EQUAL(stack.size(), 3);
  BOOST_CHECK_EQUAL(stack.get(), 3);
}

BOOST_AUTO_TEST_CASE(test_emplace_move)
{
  Stack< std::string > stack;

  std::string str = "move_stack";

  stack.emplace(std::move(str));

  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.get(), "move_stack");
}

BOOST_AUTO_TEST_CASE(test_emplace_empty)
{
  Stack< std::string > stack;

  stack.emplace("hello");

  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.get(), "hello");
}
BOOST_AUTO_TEST_SUITE_END()
