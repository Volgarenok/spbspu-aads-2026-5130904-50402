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
	auto itlist = list.cbegin();
	auto ityalist = yalist.cbegin();
	bool pass = list.size() == yalist.size();
	for (; itlist != list.cend() && ityalist != yalist.cend();)
	{
		pass = pass && *itlist == *ityalist;
		++itlist;
		++ityalist;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(op_copy_test)
{
	List< int > list;
	List< int > yalist;
	list.pushFront(22);
	list.pushFront(7);
	list.pushFront(14);
	yalist = list;
	auto itlist = list.cbegin();
	auto ityalist = yalist.cbegin();
	bool pass = list.size() == yalist.size();
	for (; itlist != list.cend() && ityalist != yalist.cend();)
	{
		pass = pass && *itlist == *ityalist;
		++itlist;
		++ityalist;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
	List< int > list;
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(1);
	List< int > yalist(std::move(list));
	auto it = yalist.cbegin();
	bool pass = yalist.size() == 3;
	pass = pass && list.isEmpty();
	for (int i = 1; it != yalist.cend(); i++)
	{
		pass = pass && *it == i;
		++it;
	}
	BOOST_TEST(pass);
}

