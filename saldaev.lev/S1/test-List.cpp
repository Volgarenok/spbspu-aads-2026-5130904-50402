#include "List.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(list_constructor)
{
  saldaev::List< size_t > list1;

  BOOST_CHECK(list1.begin() == list1.end());
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(list_rule_of_five_copy_semantics)
{
  saldaev::List< int > list1;

  list1.pushFront(1);
  list1.pushBack(2);
  list1.pushFront(0);

  saldaev::List< int > list2 = list1;

  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 0);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  saldaev::List< int > list3;
  list3 = list2;

  it = list3.begin();
  BOOST_CHECK_EQUAL(*it, 0);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  list2.pushFront(-1);
  BOOST_CHECK_NE(*list1.begin(), -1);
  list3.popFront();
  BOOST_CHECK_EQUAL(*list2.begin(), -1);

  list3 = list3;
  it = list3.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(list_rule_of_five_move_semantics)
{
  saldaev::List< int > list1;

  BOOST_CHECK(list1.begin() == list1.end());
  BOOST_CHECK_EQUAL(list1.size(), 0);

  list1.pushFront(1);
  list1.pushBack(2);
  list1.pushFront(0);

  saldaev::List< int > list2 = std::move(list1);

  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 0);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  BOOST_CHECK_EQUAL(list1.size(), 0);
  list1.pushFront(1);
  BOOST_CHECK_EQUAL(list1.size(), 1);

  saldaev::List< int > list3;
  list3 = std::move(list2);

  it = list3.begin();
  BOOST_CHECK_EQUAL(*it, 0);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  BOOST_CHECK_EQUAL(list2.size(), 0);
  list2.pushFront(1);
  BOOST_CHECK_EQUAL(list2.size(), 1);
}

BOOST_AUTO_TEST_CASE(list_adding_and_removing_nodes)
{
  saldaev::List< int > list1;

  list1.pushFront(1);
  list1.pushBack(2);
  list1.pushFront(0);

  auto it = list1.begin();
  BOOST_CHECK_EQUAL(*it, 0);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  BOOST_CHECK_EQUAL(list1.popFront(), 0);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(*list1.begin(), 1);

  BOOST_CHECK_EQUAL(list1.popBack(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 1);

  it = list1.begin();
  BOOST_CHECK_EQUAL(*it, 1);

  list1.insertAfter(it, 2);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  list1.insertBefore(it, 3);
  BOOST_CHECK_EQUAL(list1.size(), 3);
  --it;
  BOOST_CHECK_EQUAL(*it, 3);

  auto itN = list1.erase(it);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(*itN, 2);
  --itN;
  BOOST_CHECK_EQUAL(*itN, 1);

  list1.clear();
  BOOST_CHECK(list1.begin() == list1.end());
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(LCIter_test)
{
  saldaev::List< int > list1;
  const saldaev::List< int > &listc = list1;

  saldaev::List< int >::LCIter it1 = listc.begin();
  BOOST_CHECK(it1 == listc.end());

  saldaev::List< int >::LCIter itc = it1;
  BOOST_CHECK(it1 == itc);
  BOOST_CHECK(!(it1 != itc));

  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);

  saldaev::List< int >::LCIter it2 = listc.begin();
  BOOST_CHECK(it2 != listc.end());

  ++it2;
  BOOST_CHECK(it2 != listc.end());
  ++it2;
  BOOST_CHECK(it2 != listc.end());
  auto it4 = it2;
  ++it4;
  BOOST_CHECK(it4 == listc.end());
}

BOOST_AUTO_TEST_CASE(LIter_test)
{
  saldaev::List< int > list1;

  saldaev::List< int >::LIter it1 = list1.begin();
  BOOST_CHECK(it1 == list1.end());

  saldaev::List< int >::LIter itc = it1;
  BOOST_CHECK(it1 == itc);
  BOOST_CHECK(!(it1 != itc));

  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);

  saldaev::List< int >::LIter it2 = list1.begin();
  BOOST_CHECK(it2 != list1.end());
  ++it2;
  BOOST_CHECK(it2 != list1.end());
  ++it2;
  BOOST_CHECK(it2 != list1.end());
  auto it4 = it2;
  ++it4;
  BOOST_CHECK(it4 == list1.end());
}
