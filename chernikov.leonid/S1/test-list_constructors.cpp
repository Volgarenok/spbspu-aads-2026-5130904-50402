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
    original.add(42);
    original.add(13);
    List< int > copy(original);
    auto orig_it = original.begin();
    auto cp_it = copy.begin();
    BOOST_TEST(*cp_it == *orig_it);
    ++cp_it;
    ++orig_it;
    BOOST_TEST(*cp_it == *orig_it);
  }
  BOOST_AUTO_TEST_CASE(move_constructor_transfers_ownership)
  {
    List< int > original;
    original.add(42);

    List< int > moved(std::move(original));

    BOOST_TEST(moved.size() == 1);
    BOOST_TEST(*moved.begin() == 42);
    BOOST_TEST(original.empty());
  }
  BOOST_AUTO_TEST_SUITE_END()

}
