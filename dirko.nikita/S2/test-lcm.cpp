#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"
#include "calc.hpp"
#include "actions.hpp"

BOOST_AUTO_TEST_SUITE(lcmTtest)

BOOST_AUTO_TEST_CASE(lcm)
{
  std::istringstream is("3 lcm 4\n");
  dirko::Stack< long long > res;
  dirko::Queue< dirko::Queue< std::string > > expretions = dirko::input(is);
  while (!expretions.empty()) {
    dirko::Queue< std::string > post = dirko::convert(expretions.get());
    expretions.pop();
    res.push(dirko::calcExpr(post));
  }
  BOOST_CHECK_EQUAL(res.get(), 12);
}

BOOST_AUTO_TEST_SUITE_END()
