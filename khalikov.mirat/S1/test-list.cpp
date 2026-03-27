#include "list.hpp"
#include <boost/test/unit_test.hpp>

using namespace khalikov;

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
	List< int > list;
	BOOST_TEST(list.isEmpty());
}
