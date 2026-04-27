#include <boost/test/unit_test.hpp>
#include "tools.hpp"
#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE(reverseTests)

BOOST_AUTO_TEST_CASE(test_full_pipeline_with_reversal)
{
  std::istringstream input("# 12 + 3");

  lachugin::Queue< std::string > q;
  lachugin::reedInput(input, q);

  lachugin::Queue< std::string > postfix = lachugin::infixToPostfix(q);

  lachugin::Stack< long long > values;
  lachugin::counting(postfix, values);

  BOOST_CHECK_EQUAL(values.top(), 24);
}
BOOST_AUTO_TEST_SUITE_END()
