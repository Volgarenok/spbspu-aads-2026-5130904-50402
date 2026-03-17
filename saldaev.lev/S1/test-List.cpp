#include "List.hpp"
#include <boost/test/unit_test.hpp>

namespace saldaev
{
  template < typename T >
  std::ostream &operator<<(std::ostream &os, const LCIter< T > &)
  {
    os << "LCIter";
    return os;
  }

  template < typename T >
  std::ostream &operator<<(std::ostream &os, const LIter< T > &)
  {
    os << "LIter";
    return os;
  }
}

BOOST_AUTO_TEST_CASE(list_constructor)
{
  saldaev::List< size_t > list1;

  BOOST_CHECK(list1.begin() == list1.end());
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
  BOOST_CHECK_EQUAL(it.getData(), 0);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);

  list2.newHead(-1);
  BOOST_CHECK_NE(list1.begin().getData(), -1);
  list3.cutHead();
  BOOST_CHECK_EQUAL(list2.begin().getData(), -1);

  list3 = list3;
  it = list3.begin();
  BOOST_CHECK_EQUAL(it.getData(), 1);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);
}

BOOST_AUTO_TEST_CASE(list_rule_of_five_move_semantics)
{
  saldaev::List< int > list1;

  BOOST_CHECK(list1.begin() == list1.end());
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

BOOST_AUTO_TEST_CASE(list_adding_and_removing_nodes)
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

  it = list1.begin();
  BOOST_CHECK_EQUAL(it.getData(), 1);

  list1.addAfter(it, 2);
  BOOST_CHECK_EQUAL(list1.getLength(), 2);
  ++it;
  BOOST_CHECK_EQUAL(it.getData(), 2);

  list1.addBefore(it, 3);
  BOOST_CHECK_EQUAL(list1.getLength(), 3);
  --it;
  BOOST_CHECK_EQUAL(it.getData(), 3);

  auto itN = list1.cutCurrent(it);
  BOOST_CHECK_EQUAL(list1.getLength(), 2);
  BOOST_CHECK_EQUAL(itN.getData(), 2);
  --itN;
  BOOST_CHECK_EQUAL(itN.getData(), 1);

  list1.clear();
  BOOST_CHECK(list1.begin() == list1.end());
  BOOST_CHECK_EQUAL(list1.getLength(), 0);
}

BOOST_AUTO_TEST_CASE(LCIter_test)
{
  saldaev::List< int > list1;
  const saldaev::List< int > &listc = list1;

  saldaev::LCIter< int > it1 = listc.begin();
  BOOST_CHECK(it1 == list1.end());
  BOOST_CHECK_EQUAL(it1.isValid(), false);

  saldaev::LCIter< int > itc = it1;
  BOOST_CHECK(it1 == itc);
  BOOST_CHECK(it1 != itc == false);

  it1++;
  BOOST_CHECK(it1 == itc);
  itc--;
  BOOST_CHECK(it1 == itc);
  ++it1;
  BOOST_CHECK(it1 == itc);
  --itc;
  BOOST_CHECK(it1 == itc);

  list1.newTail(1);
  list1.newTail(2);
  list1.newTail(3);

  saldaev::LCIter< int > it2 = listc.begin();
  BOOST_CHECK_EQUAL(it2.isValid(), true);
  BOOST_CHECK_EQUAL(it2.hasPrev(), false);
  BOOST_CHECK_EQUAL(it2.hasNext(), true);
  it2++;
  BOOST_CHECK_EQUAL(it2.isValid(), true);
  BOOST_CHECK_EQUAL(it2.hasPrev(), true);
  BOOST_CHECK_EQUAL(it2.hasNext(), true);
  it2++;
  BOOST_CHECK_EQUAL(it2.isValid(), true);
  BOOST_CHECK_EQUAL(it2.hasPrev(), true);
  BOOST_CHECK_EQUAL(it2.hasNext(), false);
  it2++;
  BOOST_CHECK_EQUAL(it2.isValid(), false);
  BOOST_CHECK_EQUAL(it2.hasPrev(), false);
  BOOST_CHECK_EQUAL(it2.hasNext(), false);
}

BOOST_AUTO_TEST_CASE(LIter_test)
{
  saldaev::List< int > list1;

  saldaev::LIter< int > it1 = list1.begin();
  BOOST_CHECK(it1 == list1.end());
  BOOST_CHECK_EQUAL(it1.isValid(), false);

  saldaev::LIter< int > itc = it1;
  BOOST_CHECK(it1 == itc);
  BOOST_CHECK(it1 != itc == false);

  it1++;
  BOOST_CHECK(it1 == itc);
  itc--;
  BOOST_CHECK(it1 == itc);
  ++it1;
  BOOST_CHECK(it1 == itc);
  --itc;
  BOOST_CHECK(it1 == itc);

  list1.newTail(1);
  list1.newTail(2);
  list1.newTail(3);

  saldaev::LIter< int > it2 = list1.begin();
  BOOST_CHECK_EQUAL(it2.isValid(), true);
  BOOST_CHECK_EQUAL(it2.hasPrev(), false);
  BOOST_CHECK_EQUAL(it2.hasNext(), true);
  it2++;
  BOOST_CHECK_EQUAL(it2.isValid(), true);
  BOOST_CHECK_EQUAL(it2.hasPrev(), true);
  BOOST_CHECK_EQUAL(it2.hasNext(), true);
  it2++;
  BOOST_CHECK_EQUAL(it2.isValid(), true);
  BOOST_CHECK_EQUAL(it2.hasPrev(), true);
  BOOST_CHECK_EQUAL(it2.hasNext(), false);
  it2++;
  BOOST_CHECK_EQUAL(it2.isValid(), false);
  BOOST_CHECK_EQUAL(it2.hasPrev(), false);
  BOOST_CHECK_EQUAL(it2.hasNext(), false);
}
