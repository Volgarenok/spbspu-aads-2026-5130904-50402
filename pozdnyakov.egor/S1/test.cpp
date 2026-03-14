#define BOOST_TEST_MODULE S1_TESTS
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include "process.hpp"
#include <initializer_list>
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(testListInsertAndIterators)
{
  pozdnyakov::List< int > list;
  BOOST_TEST(list.empty());

  list.pushFront(10);
  BOOST_TEST(list.front() == 10);

  list.insertAfter(list.begin(), 20);

  int expected[] = {10, 20};
  int i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_TEST(*it == expected[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(testListCopyExceptionSafety)
{
  pozdnyakov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);

  pozdnyakov::List< int > list2 = list1;
  BOOST_TEST(!list2.empty());
  BOOST_TEST(list2.front() == 1);

  list2.insertAfter(list2.begin(), 3);

  auto it = list1.begin();
  ++it;
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()

pozdnyakov::List< pozdnyakov::ValueType > makeList(std::initializer_list< pozdnyakov::ValueType > vals)
{
  pozdnyakov::List< pozdnyakov::ValueType > lst;
  pozdnyakov::LIter< pozdnyakov::ValueType > tail = lst.end();
  for (auto v : vals) {
    if (lst.empty()) {
      lst.pushFront(v);
      tail = lst.begin();
    } else {
      lst.insertAfter(tail, v);
      ++tail;
    }
  }
  return lst;
}

BOOST_AUTO_TEST_SUITE(ProcessTests)

BOOST_AUTO_TEST_CASE(testEmptySequences)
{
  pozdnyakov::List< pozdnyakov::NamedSequence > seqs;

  pozdnyakov::ProcessResult res = pozdnyakov::buildInterleavedRows(seqs);

  BOOST_TEST(res.rows.empty());
  BOOST_TEST(res.sums.empty());
}

BOOST_AUTO_TEST_CASE(testSequencesWithoutNumbers)
{
  pozdnyakov::List< pozdnyakov::NamedSequence > seqs;
  seqs.pushFront({"second", makeList({})});
  seqs.pushFront({"first", makeList({})});

  pozdnyakov::ProcessResult res = pozdnyakov::buildInterleavedRows(seqs);

  BOOST_TEST(res.rows.empty());
  BOOST_TEST(res.sums.empty());
}

BOOST_AUTO_TEST_CASE(testSingleSequence)
{
  pozdnyakov::List< pozdnyakov::NamedSequence > seqs;
  seqs.pushFront({"single", makeList({10, 20})});

  pozdnyakov::ProcessResult res = pozdnyakov::buildInterleavedRows(seqs);

  pozdnyakov::ValueType expectedSums[] = {10, 20};
  int i = 0;
  for (auto it = res.sums.begin(); it != res.sums.end(); ++it) {
    BOOST_TEST(*it == expectedSums[i++]);
  }
  BOOST_TEST(i == 2);
}

BOOST_AUTO_TEST_CASE(testProcessLogicOverflow)
{
  pozdnyakov::List< pozdnyakov::NamedSequence > seqs;
  seqs.pushFront({"list2", makeList({2})});
  seqs.pushFront({"list1", makeList({18446744073709551615ULL})});

  BOOST_CHECK_THROW(pozdnyakov::buildInterleavedRows(seqs), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
