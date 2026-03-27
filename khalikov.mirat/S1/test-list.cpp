#include "list.hpp"
#include <boost/test/unit_test.hpp>

using namespace khalikov;

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
	List< int > list;
	BOOST_TEST(list.isEmpty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
	List< int > list;
	list.pushFront(1);
	list.pushFront(5);
	list.pushFront(3);
	List< int > yalist(list);
	auto itlist = list.begin();
	auto ityalist = yalist.begin();
	bool pass = itlist == ityalist;
	while (itlist !=)
	pass = pass && list.h->next->val == yalist.h->next->val;
	BOOST_TEST(pass);
}

