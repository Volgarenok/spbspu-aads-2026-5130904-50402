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
	bool pass = list.size() == yalist.size();
	auto yait = yalist.cbegin();
	int arr[3] = {3, 5, 1};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *yait == arr[i];
		++yait;
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
	auto yait = yalist.cbegin();
	bool pass = list.size() == yalist.size();
	int arr[3] = {14, 7, 22};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *yait == arr[i];
		++yait;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
	List< int > list;
	list.pushFront(22);
	list.pushFront(7);
	list.pushFront(14);
	List< int > yalist(std::move(list));
	auto yait = yalist.cbegin();
	bool pass = yalist.size() == 3;
	pass = pass && list.isEmpty();
	int arr[3] = {14, 7, 22};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *yait == arr[i];
		++yait;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(move_op_test)
{
	List< int > list;
	List< int > yalist;
	list.pushFront(33);
	list.pushFront(24);
	list.pushFront(11);
	yalist = std::move(list);
	auto yait = yalist.cbegin();
	bool pass = yalist.size() == 3;
	pass = pass && list.isEmpty();
	int arr[3] = {11, 24, 33};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *yait == arr[i];
		++yait;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
	List< int > list;
	List< int > yalist;
	list.pushFront(3);
	list.pushFront(25);
	list.pushFront(12);
	yalist.pushFront(15);
	yalist.pushFront(7);
	list.swap(yalist);
	auto yait = yalist.cbegin();
	auto it = list.cbegin();
	bool pass = yalist.size() == 3;
	pass = pass && list.size() == 2;
	int arr[] = {7, 15};
	int yaarr[] = {12, 25, 3};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	for (size_t i = 0; i < yalist.size(); i++)
	{
		pass = pass && *yait == yaarr[i];
		++yait;
	}
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
	int j = 1;
	for (size_t i = 0; i < list.size(); ++i)
	{
		pass = pass && *it == j++;
		++it;
	}
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

BOOST_AUTO_TEST_CASE(push_front_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	auto it = list.cbegin();
	pass = pass && list.size() == 1;
	pass = pass && *it == 3;
	list.pushFront(2);
	pass = pass && list.size() == 2;
	it = list.cbegin();
	pass = pass && *it == 2;
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(7);
	list.pushFront(2);
	list.pushFront(11);
	list.pushFront(9);
	list.pushFront(6);
	pass = pass && list.size() == 7;
	auto it = list.begin();
	++it;
	++it;
	++it;
	list.erase(it);
	pass = pass && list.size() == 6;
	it = list.begin();
	int arr[6] = {6, 9, 11, 7, 2, 3};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(5);
	list.pushFront(5);
	list.pushFront(11);
	list.pushFront(5);
	list.pushFront(5);
	list.remove(5);
	pass = pass && list.size() == 3;
	auto it = list.cbegin();
	int arr[3] = {11, 2, 3};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(sort_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(4);
	list.pushFront(19);
	list.pushFront(11);
	list.pushFront(25);
	list.pushFront(20);
	list.sort();
	auto it = list.cbegin();
	int arr[7] = {2, 3, 4, 11, 19, 20, 25};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(unique_sort_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(4);
	list.pushFront(3);
	list.pushFront(5);
	list.pushFront(2);
	list.pushFront(7);
	list.uniqueSort();
	auto it = list.cbegin();
	int arr[5] = {2, 3, 4, 5, 7};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(reverse_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushFront(3);
	list.pushFront(2);
	list.pushFront(4);
	list.pushFront(3);
	list.pushFront(5);
	list.pushFront(2);
	list.pushFront(7);
	list.reverse();
	auto it = list.cbegin();
	int arr[7] = {3, 2, 4, 3, 5, 2, 7};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushBack(1);
	list.pushBack(2);
	list.pushBack(3);
	pass = pass && list.size() == 3;
	auto it = list.cbegin();
	int arr[] = {1, 2, 3};
	for (size_t i = 0; i < list.size(); i++)
	{
		pass = pass && *it == arr[i];
		++it;
	}
	BOOST_TEST(pass);
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
	List< int > list;
	bool pass = list.isEmpty();
	list.pushBack(1);
	list.pushBack(2);
	list.pushBack(3);
	pass = pass && list.size() == 3;
	list.popBack();
	pass = pass && list.size() == 2;
	auto it = list.cbegin();
	++it;
	pass = pass && *it == 2;
	BOOST_TEST(pass);
}

