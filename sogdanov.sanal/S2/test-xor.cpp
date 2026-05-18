#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "operators.hpp"
BOOST_AUTO_TEST_SUITE(xor_suite)
BOOST_AUTO_TEST_CASE(xor_basic) {
  BOOST_CHECK_EQUAL(sogdanov::evaluate("5 ^ 3"), 6);
  BOOST_CHECK_EQUAL(sogdanov::evaluate("0 ^ 0"), 0);
  BOOST_CHECK_EQUAL(sogdanov::evaluate("7 ^ 7"), 0);
  BOOST_CHECK_EQUAL(sogdanov::evaluate("15 ^ 9"), 6);
}
BOOST_AUTO_TEST_CASE(xor_with_parens) {
  BOOST_CHECK_EQUAL(sogdanov::evaluate("( 1 + 2 ) ^ ( 4 - 2 )"), 1);
  BOOST_CHECK_EQUAL(sogdanov::evaluate("( 8 / 2 ) ^ ( 3 * 1 )"), 7);
}
BOOST_AUTO_TEST_CASE(xor_precedence) {
  BOOST_CHECK_EQUAL(sogdanov::evaluate("1 + 2 ^ 3"), 0);
  BOOST_CHECK_EQUAL(sogdanov::evaluate("4 - 1 ^ 2"), 1);
  BOOST_CHECK_EQUAL(sogdanov::evaluate("2 * 3 ^ 1 + 0"), 7);
}
BOOST_AUTO_TEST_CASE(xor_chained) {
  BOOST_CHECK_EQUAL(sogdanov::evaluate("1 ^ 2 ^ 3"), 0);
  BOOST_CHECK_EQUAL(sogdanov::evaluate("5 ^ 0"), 5);
}
BOOST_AUTO_TEST_SUITE_END()
