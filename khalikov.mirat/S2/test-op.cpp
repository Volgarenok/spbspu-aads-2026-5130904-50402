#include "op.hpp"
#include <boost/test/unit_test.hpp>

using namespace khalikov;

BOOST_AUTO_TEST_SUITE(OpTests)

BOOST_AUTO_TEST_CASE(op_test)
{
  BOOST_CHECK_EQUAL(op(1, 2, "#"), 12);
  BOOST_CHECK_EQUAL(op(100, 5, "+"), 105);
  BOOST_CHECK_EQUAL(op(99, 23, "-"), 76);
  BOOST_CHECK_EQUAL(op(12, 3, "*"), 36);
  BOOST_CHECK_EQUAL(op(100, 5, "/"), 20);
  BOOST_CHECK_EQUAL(op(34, 23, "%"), 11);
}

BOOST_AUTO_TEST_SUITE_END()
