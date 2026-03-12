#include "List.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(list_constructor)
{
  saldaev::List< int > list1;

  BOOST_CHECK_EQUAL(list1.begin(), list1.end());
  BOOST_CHECK_EQUAL(list1.getLength(), 0);
}

BOOST_AUTO_TEST_CASE(list_rule_of_five_copy_semantics)
{
  saldaev::List< int > list1;

  list1.newHead(1);
  list1.newTail(2);
  list1.newHead(0);

  saldaev::List< int > list2 = list1;

  auto it = list2.begin();
  BOOST_CHECK_EQUAL(it.getData(), 0);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);

  saldaev::List< int > list3;
  list3 = list2;

  it = list3.begin();
  BOOST_CHECK_EQUAL(it.getData(), -1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 0);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 1);

  list2.newHead(-1);
  BOOST_CHECK_NE(list1.begin().getData(), -1);
  list3.cutHead();
  BOOST_CHECK_EQUAL(list2.begin().getData(), -1);

  list3 = list3;
  it = list3.begin();
  BOOST_CHECK_EQUAL(it.getData(), 0);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);
}

BOOST_AUTO_TEST_CASE(list_rule_of_five_move_semantics)
{
  saldaev::List< int > list1;

  BOOST_CHECK_EQUAL(list1.begin(), list1.end());
  BOOST_CHECK_EQUAL(list1.getLength(), 0);

  list1.newHead(1);
  list1.newTail(2);
  list1.newHead(0);

  saldaev::List< int > list2 = std::move(list1);

  auto it = list2.begin();
  BOOST_CHECK_EQUAL(it.getData(), 0);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);

  BOOST_CHECK_EQUAL(list1.getLength(), 0);
  list1.newHead(1);
  BOOST_CHECK_EQUAL(list1.getLength(), 1);

  saldaev::List< int > list3;
  list3 = std::move(list2);

  it = list3.begin();
  BOOST_CHECK_EQUAL(it.getData(), 0);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);

  BOOST_CHECK_EQUAL(list2.getLength(), 0);
  list2.newHead(1);
  BOOST_CHECK_EQUAL(list2.getLength(), 1);
}

BOOST_AUTO_TEST_CASE(list_adding_and_removing_nods)
{
  saldaev::List< int > list1;

  list1.newHead(1);
  list1.newTail(2);
  list1.newHead(0);

  auto it = list1.begin();
  BOOST_CHECK_EQUAL(it.getData(), 0);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);

  list1.cutHead();
  BOOST_CHECK_EQUAL(list1.getLength(), 2);
  BOOST_CHECK_EQUAL(list1.begin().getData(), 1);

  list1.cutTail();
  BOOST_CHECK_EQUAL(list1.getLength(), 1);
  BOOST_CHECK_EQUAL((list1.end()).getData(), 1);

  it = list1.begin();

  list1.addAfter(it, 2);
  BOOST_CHECK_EQUAL(list1.getLength(), 2);
  BOOST_CHECK_EQUAL((++it).getData(), 2);

  list1.addBefore(it, 3);
  BOOST_CHECK_EQUAL(list1.getLength(), 3);
  BOOST_CHECK_EQUAL((--it).getData(), 3);

  auto itN = list1.cutCurrent(it);
  BOOST_CHECK_EQUAL(list1.getLength(), 2);
  BOOST_CHECK_EQUAL(itN.getData(), 2);
  itN--;
  BOOST_CHECK_EQUAL(itN.getData(), 1);

  list1.clear();
  BOOST_CHECK_EQUAL(list1.begin(), list1.end());
  BOOST_CHECK_EQUAL(list1.getLength(), 0);
}

BOOST_AUTO_TEST_CASE(LCIter)
{
  saldaev::List< int > list1;
  const saldaev::List< int > &listc = list1;

  saldaev::LCIter< int > it = listc.begin();
  BOOST_CHECK_EQUAL(it, list1.end());
  BOOST_CHECK_EQUAL(it.isValid(), false);

  saldaev::LCIter< int > itc = it;
  BOOST_CHECK_EQUAL(it == itc, true);
  BOOST_CHECK_EQUAL(it != itc, false);

  it++;
  BOOST_CHECK_EQUAL(it, itc);
  itc--;
  BOOST_CHECK_EQUAL(it, itc);
  ++it;
  BOOST_CHECK_EQUAL(it, itc);
  --itc;
  BOOST_CHECK_EQUAL(it, itc);

  list1.newTail(1);
  list1.newTail(2);
  list1.newTail(3);

  saldaev::LCIter< int > it = listc.begin();
  BOOST_CHECK_EQUAL(it.isValid(), true);
  BOOST_CHECK_EQUAL(it.hasPrev(), false);
  BOOST_CHECK_EQUAL(it.hasNext(), true);
  it++;
  BOOST_CHECK_EQUAL(it.isValid(), true);
  BOOST_CHECK_EQUAL(it.hasPrev(), true);
  BOOST_CHECK_EQUAL(it.hasNext(), true);
  it++;
  BOOST_CHECK_EQUAL(it.isValid(), true);
  BOOST_CHECK_EQUAL(it.hasPrev(), true);
  BOOST_CHECK_EQUAL(it.hasNext(), false);
  it++;
  BOOST_CHECK_EQUAL(it.isValid(), false);
  BOOST_CHECK_EQUAL(it.hasPrev(), false);
  BOOST_CHECK_EQUAL(it.hasNext(), false);
}

BOOST_AUTO_TEST_CASE(LIter)
{
  saldaev::List< int > list1;

  saldaev::LIter< int > it = list1.begin();
  BOOST_CHECK_EQUAL(it, list1.end());
  BOOST_CHECK_EQUAL(it.isValid(), false);

  saldaev::LIter< int > itc = it;
  BOOST_CHECK_EQUAL(it == itc, true);
  BOOST_CHECK_EQUAL(it != itc, false);

  it++;
  BOOST_CHECK_EQUAL(it, itc);
  itc--;
  BOOST_CHECK_EQUAL(it, itc);
  ++it;
  BOOST_CHECK_EQUAL(it, itc);
  --itc;
  BOOST_CHECK_EQUAL(it, itc);

  list1.newTail(1);
  list1.newTail(2);
  list1.newTail(3);

  saldaev::LIter< int > it = list1.begin();
  BOOST_CHECK_EQUAL(it.isValid(), true);
  BOOST_CHECK_EQUAL(it.hasPrev(), false);
  BOOST_CHECK_EQUAL(it.hasNext(), true);
  it++;
  BOOST_CHECK_EQUAL(it.isValid(), true);
  BOOST_CHECK_EQUAL(it.hasPrev(), true);
  BOOST_CHECK_EQUAL(it.hasNext(), true);
  it++;
  BOOST_CHECK_EQUAL(it.isValid(), true);
  BOOST_CHECK_EQUAL(it.hasPrev(), true);
  BOOST_CHECK_EQUAL(it.hasNext(), false);
  it++;
  BOOST_CHECK_EQUAL(it.isValid(), false);
  BOOST_CHECK_EQUAL(it.hasPrev(), false);
  BOOST_CHECK_EQUAL(it.hasNext(), false);
}
