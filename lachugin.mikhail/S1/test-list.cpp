#include <stdexcept>
#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

using namespace lachugin;

BOOST_AUTO_TEST_SUITE(ListMethodsTests)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  List< int > list;
  BOOST_CHECK(list.begin() == list.end());
}



BOOST_AUTO_TEST_SUITE_END()
