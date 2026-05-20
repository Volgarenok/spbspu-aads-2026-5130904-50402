#define BOOST_TEST_MODULE list_tests
#include <boost/test/included/unit_test.hpp>

#include "list.hpp"

BOOST_AUTO_TEST_CASE(insert_and_iterate)
{
  matveev::List<int> list;

  auto it = list.beforeBegin();
  it = list.insertAfter(it,1);
  it = list.insertAfter(it,2);
  it = list.insertAfter(it,3);

  auto iter = list.begin();

  BOOST_CHECK(*iter == 1);
  ++iter;
  BOOST_CHECK(*iter == 2);
  ++iter;
  BOOST_CHECK(*iter == 3);
}

BOOST_AUTO_TEST_CASE(erase_after)
{
  matveev::List<int> list;

  auto it = list.beforeBegin();
  it = list.insertAfter(it,1);
  it = list.insertAfter(it,2);
  it = list.insertAfter(it,3);

  auto first = list.begin();
  list.eraseAfter(first);

  auto iter = list.begin();
  BOOST_CHECK(*iter == 1);
  ++iter;
  BOOST_CHECK(*iter == 3);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  matveev::List<int> list;

  auto it = list.beforeBegin();
  it = list.insertAfter(it,5);
  it = list.insertAfter(it,6);

  list.clear();

  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  matveev::List<int> list;

  auto it = list.beforeBegin();
  it = list.insertAfter(it,10);
  it = list.insertAfter(it,20);

  matveev::List<int> copy(list);

  auto a = list.begin();
  auto b = copy.begin();

  BOOST_CHECK(*a == *b);
  ++a;
  ++b;
  BOOST_CHECK(*a == *b);
}

BOOST_AUTO_TEST_CASE(assignment_operator)
{
  matveev::List<int> a;
  auto it = a.beforeBegin();
  it = a.insertAfter(it,7);
  it = a.insertAfter(it,8);

  matveev::List<int> b;
  b = a;

  auto ia = a.begin();
  auto ib = b.begin();

  BOOST_CHECK(*ia == *ib);
  ++ia;
  ++ib;
  BOOST_CHECK(*ia == *ib);
}

BOOST_AUTO_TEST_CASE(splice_after_one_element)
{
  matveev::List< int > a;
  auto aIt = a.beforeBegin();
  aIt = a.insertAfter(aIt, 1);
  aIt = a.insertAfter(aIt, 3);

  matveev::List< int > b;
  auto bIt = b.beforeBegin();
  bIt = b.insertAfter(bIt, 2);

  auto pos = a.begin();
  a.spliceAfter(pos, b, b.beforeBegin());

  auto it = a.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);

  BOOST_CHECK(b.begin() == b.end());
}

BOOST_AUTO_TEST_CASE(merge_sorted_lists)
{
  matveev::List< int > a;
  auto aIt = a.beforeBegin();
  aIt = a.insertAfter(aIt, 1);
  aIt = a.insertAfter(aIt, 3);
  aIt = a.insertAfter(aIt, 5);

  matveev::List< int > b;
  auto bIt = b.beforeBegin();
  bIt = b.insertAfter(bIt, 2);
  bIt = b.insertAfter(bIt, 4);
  bIt = b.insertAfter(bIt, 6);

  a.merge(b);

  auto it = a.begin();
  for (int expected = 1; expected <= 6; ++expected)
  {
    BOOST_CHECK(*it == expected);
    ++it;
  }

  BOOST_CHECK(b.begin() == b.end());
}

BOOST_AUTO_TEST_CASE(sort_list)
{
  matveev::List< int > list;
  auto it = list.beforeBegin();
  it = list.insertAfter(it, 4);
  it = list.insertAfter(it, 1);
  it = list.insertAfter(it, 3);
  it = list.insertAfter(it, 2);

  list.sort();

  auto iter = list.begin();
  for (int expected = 1; expected <= 4; ++expected)
  {
    BOOST_CHECK(*iter == expected);
    ++iter;
  }
}

BOOST_AUTO_TEST_CASE(partition_list)
{
  matveev::List< int > list;
  auto it = list.beforeBegin();
  it = list.insertAfter(it, 1);
  it = list.insertAfter(it, 2);
  it = list.insertAfter(it, 3);
  it = list.insertAfter(it, 4);

  auto border = list.partition([](int value)
  {
    return value % 2 == 0;
  });

  auto iter = list.begin();
  BOOST_CHECK(*iter == 2);
  ++iter;
  BOOST_CHECK(*iter == 4);
  BOOST_CHECK(iter == border);
  ++iter;
  BOOST_CHECK(*iter == 1);
  ++iter;
  BOOST_CHECK(*iter == 3);
}
