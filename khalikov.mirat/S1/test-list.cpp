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

BOOST_AUTO_TEST_CASE(copy_op_test)
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

BOOST_AUTO_TEST_CASE(move_op_test)
{
	List< int > list;
	List< int > yalist;
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(1);
	yalist = std::move(list);
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


BOOST_AUTO_TEST_CASE(swap_test)
{
	List< int > list;
	List< int > yalist;
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(1);
	yalist.pushFront(2);
	yalist.pushFront(1);
	list.swap(yalist);
	auto yait = yalist.cbegin();
	auto it = list.cbegin();
	bool pass = yalist.size() == 3;
	pass = pass && list.size() == 2;
	for (int i = 1; it != list.cend(); i++)
	{
		pass = pass && *it == *yait;
		++it;
		++yait;
	}
	pass = pass && *yait == 3;
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(size_test)
{
	List< int > list;
	bool pass = !list.size();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(1);
	pass = pass && list.size() == 3;
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(1);
	pass = pass && list.size() == 3;
	list.clear();
	pass = pass && list.isEmpty();
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(erase_after_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(1);
	pass = pass && list.size() == 3;
	auto it = list.begin();
	list.eraseAfter(it);
	pass = pass && list.size() == 2;
	it = list.begin();
	pass = pass && *it == 1;
	++it;
	pass = pass && *it == 3;
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(insert_after_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(4);
	list.pushFront(3);
	list.pushFront(1);
	pass = pass && list.size() == 3;
	auto it = list.begin();
	list.insertAfter(it, 2);
	pass = pass && list.size() == 4;
	it = list.begin();
	for (int i = 1; it != list.end(); ++i)
	{
		pass = pass && *it == i;
		++it;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(merge_test)
{
	List< int > list;
	List< int > yalist;
	list.pushFront(9);
	list.pushFront(7);
	list.pushFront(10);
	yalist.pushFront(3);
	yalist.pushFront(12);
	yalist.sort();
	list.sort();
	list.merge(yalist);
	auto it = list.cbegin();
	bool pass = list.size() == 5;
	int arr[5] = {3, 7, 9, 10, 12};
	for (int i = 0; it != list.cend(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	pass = pass && yalist.size() == 0;
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(1);
	pass = pass && list.size() == 3;
	list.popFront();
	pass = pass && list.size() == 2;
	auto it = list.cbegin();
	pass = pass && *it == 2;
	BOOST_TEST(pass);
}

