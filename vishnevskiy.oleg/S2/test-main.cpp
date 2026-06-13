#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>
#include "QueueImpl.hpp"
#include "StackImpl.hpp"

BOOST_AUTO_TEST_CASE(queue_test_empty)
{
  vishnevskiy::Queue<int> t;
  BOOST_CHECK_THROW(t.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(stack_test_empty)
{
  vishnevskiy::Stack<int> t;
  BOOST_CHECK_THROW(t.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(stack_test_push)
{
  vishnevskiy::Stack<int> t;
  t.push(6);
  BOOST_TEST(t.drop() == 6);
}

BOOST_AUTO_TEST_CASE(queue_test_push)
{
  vishnevskiy::Queue<int> t;
  t.push(6);
  BOOST_TEST(t.drop() == 6);
}

