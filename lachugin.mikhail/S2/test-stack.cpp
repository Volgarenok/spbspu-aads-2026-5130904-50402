#include <boost/test/included/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(LachuginStackTests)

BOOST_AUTO_TEST_CASE(test_stack_basic)
{
  lachugin::Stack< int > s;

  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);

  s.push(1);
  s.push(2);
  s.push(3);

  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 3);

  BOOST_CHECK_EQUAL(s.top(), 3);
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), 2);
  s.pop();

  BOOST_CHECK_EQUAL(s.top(), 1);
  s.pop();

  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_SUITE_END()
