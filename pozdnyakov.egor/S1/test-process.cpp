#include <boost/test/unit_test.hpp>
#include "process.hpp"
#include "list.hpp"
#include <stdexcept>
#include <cstddef>

using namespace pozdnyakov;

List< ValueType > makeList(const ValueType *vals, std::size_t size)
{
  List< ValueType > lst;
  LIter< ValueType > tail = lst.end();
  for (std::size_t i = 0; i < size; ++i) {
    if (lst.empty()) {
      lst.pushFront(vals[i]);
      tail = lst.begin();
    } else {
      lst.insertAfter(tail, vals[i]);
      ++tail;
    }
  }
  return lst;
}

BOOST_AUTO_TEST_SUITE(ProcessTests)

BOOST_AUTO_TEST_CASE(testEmptySequences)
{
  List< NamedSequence > seqs;

  ProcessResult res = buildInterleavedRows(seqs);

  BOOST_CHECK(res.rows.empty());
  BOOST_CHECK(res.sums.empty());
}

BOOST_AUTO_TEST_CASE(testSequencesWithoutNumbers)
{
  List< NamedSequence > seqs;
  seqs.pushFront({"second", List< ValueType >()});
  seqs.pushFront({"first", List< ValueType >()});

  ProcessResult res = buildInterleavedRows(seqs);

  BOOST_CHECK(res.rows.empty());
  BOOST_CHECK(res.sums.empty());
}

BOOST_AUTO_TEST_CASE(testSingleSequence)
{
  List< NamedSequence > seqs;
  ValueType vals[] = {10, 20};
  seqs.pushFront({"single", makeList(vals, 2)});

  ProcessResult res = buildInterleavedRows(seqs);

  ValueType expectedSums[] = {10, 20};
  int i = 0;
  for (auto it = res.sums.begin(); it != res.sums.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expectedSums[i++]);
  }
  BOOST_CHECK_EQUAL(i, 2);
}

BOOST_AUTO_TEST_CASE(testProcessLogicOverflow)
{
  List< NamedSequence > seqs;

  ValueType val2[] = {2};
  seqs.pushFront({"list2", makeList(val2, 1)});

  ValueType val1[] = {18446744073709551615ULL};
  seqs.pushFront({"list1", makeList(val1, 1)});

  BOOST_CHECK_THROW(buildInterleavedRows(seqs), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
