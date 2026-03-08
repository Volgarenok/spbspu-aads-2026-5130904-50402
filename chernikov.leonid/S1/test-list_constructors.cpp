#include <boost/test/unit_test.hpp>
#include "List.hpp"

namespace chernikov {

  BOOST_AUTO_TEST_SUITE(Constructors)

  BOOST_AUTO_TEST_CASE(default_constructor)
  {
    List< int > list;
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);
    BOOST_TEST(list.begin() == list.end());
  }
  BOOST_AUTO_TEST_CASE(copy_costructor)
  {
    List< int > original;
    List< int > copy(original);
    auto orig_it = original.begin();
    auto cp_it = copy.begin();
    BOOST_TEST(*orig_it == *cp_it);
  }
  BOOST_AUTO_TEST_SUITE_END()

}
