#define BOOST_TEST_MODULE S0
#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace permyakov;

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  List < int > listNum;
  BOOST_CHECK_EQUAL(listNum.size(), 0);
  BOOST_CHECK_EQUAL(listNum.front(), 0);
  List < bool > listBool;
  BOOST_CHECK_EQUAL(listBool.size(), 0);
  BOOST_CHECK_EQUAL(listBool.front(), false);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List < int > frstList;
  frstList.push_front(2);
  frstList.push_front(1);
  List < int > scndList(frstList);
  BOOST_CHECK_EQUAL(frstList.size(), scndList.size());
  LIter < int > frstIter = frstList.begin();
  LIter < int > frstEndIter = frstList.end();
  LIter < int > scndIter = scndList.begin();
  while(frstIter != frstEndIter) {
    BOOST_CHECK(frstIter == scndIter);
    ++frstIter;
    ++scndIter;
  }
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  List < int > frstList;
  frstList.push_front(2);
  frstList.push_front(1);
  List < int > scndList(std::move(frstList));
  BOOST_CHECK_EQUAL(frstList.size(), 0);
  BOOST_CHECK_EQUAL(scndList.size(), 2);
  BOOST_CHECK_EQUAL(scndList.front(),1);
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
  List < int > frstList;
  frstList.push_front(2);
  frstList.push_front(1);
  List < int > scndList = frstList;
  BOOST_CHECK_EQUAL(frstList.size(), scndList.size());
  LIter < int > frstIter = frstList.begin();
  LIter < int > frstEndIter = frstList.end();
  LIter < int > scndIter = scndList.begin();
  while(frstIter != frstEndIter) {
    BOOST_CHECK(frstIter == scndIter);
    ++frstIter;
    ++scndIter;
  }
}

BOOST_AUTO_TEST_CASE(test_move_operator)
{
  List < int > frstList;
  frstList.push_front(2);
  frstList.push_front(1);
  List < int > scndList =std::move(frstList);
  BOOST_CHECK_EQUAL(frstList.size(), 0);
  BOOST_CHECK_EQUAL(scndList.size(), 2);
  BOOST_CHECK_EQUAL(scndList.front(),1);
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  {
    List < int > list;
    list.push_front(1);
  }
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  list.push_front(0);
  LIter < int > bItList = list.begin();
  LIter < int > eItList = list.end();
  ++bItList;
  list.erase(bItList);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 0);
  BOOST_CHECK(bItList == eItList);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List < int > list;
  LIter < int > itList = list.begin();
  list.push_front(0);
  list.push_front(0);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  LIter < int > sItList = list.begin();
  BOOST_CHECK(itList == sItList);
}

BOOST_AUTO_TEST_CASE(test_begin)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  LIter < int > itList = list.begin();
  BOOST_CHECK_EQUAL(*itList, 1);
}

BOOST_AUTO_TEST_CASE(test_end)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  LIter < int > itList = list.end();
  BOOST_CHECK_EQUAL(*itList, 2);
}

BOOST_AUTO_TEST_CASE(test_beginC)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  LCIter < int > itList = list.beginC();
  BOOST_CHECK_EQUAL(*itList, 1);
}

BOOST_AUTO_TEST_CASE(test_endC)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  LCIter < int > itList = list.endC();
  BOOST_CHECK_EQUAL(*itList, 2);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  List < int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.push_front(0);
  BOOST_CHECK_EQUAL(list.size(), 1);
  list.push_front(0);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List < int > list;
  list.push_front(10);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 10);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  list.pop_front();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_SUITE_END()