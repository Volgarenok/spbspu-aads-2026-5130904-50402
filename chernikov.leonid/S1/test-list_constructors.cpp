#include <boost/test/unit_test.hpp>
#include "List.hpp"

namespace chernikov
{

  BOOST_AUTO_TEST_SUITE(Constructors)

  BOOST_AUTO_TEST_CASE(default_constructor_empty_list)
  {
    List< int > list;
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);
    BOOST_TEST(list.begin() == list.end());
  }
  BOOST_AUTO_TEST_SUITE_END()

}