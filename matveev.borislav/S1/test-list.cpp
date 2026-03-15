#define BOOST_TEST_MODULE list_tests
#include <boost/test/included/unit_test.hpp>

#include "list.hpp"

BOOST_AUTO_TEST_CASE(insert_and_iterate)
{
  matveev::List<int> list;

  auto it=list.beforeBegin();

  it=list.insertAfter(it,1);
  it=list.insertAfter(it,2);
  it=list.insertAfter(it,3);

  auto iter=list.begin();

  BOOST_CHECK(*iter==1);
  ++iter;
  BOOST_CHECK(*iter==2);
  ++iter;
  BOOST_CHECK(*iter==3);
}

BOOST_AUTO_TEST_CASE(erase_after)
{
  matveev::List<int> list;

  auto it=list.beforeBegin();
  it=list.insertAfter(it,1);
  it=list.insertAfter(it,2);
  it=list.insertAfter(it,3);

  auto first=list.begin();
  list.eraseAfter(first);

  auto iter=list.begin();
  BOOST_CHECK(*iter==1);
  ++iter;
  BOOST_CHECK(*iter==3);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  matveev::List<int> list;

  auto it=list.beforeBegin();
  it=list.insertAfter(it,5);
  it=list.insertAfter(it,6);

  list.clear();

  BOOST_CHECK(list.begin()==list.end());
}
