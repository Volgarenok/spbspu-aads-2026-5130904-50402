#include <boost/test/unit_test.hpp>
#include "process.hpp"
#include "list.hpp"
#include <vector>

BOOST_AUTO_TEST_SUITE(ProcessTests)

pozdnyakov::List< pozdnyakov::ValueType > makeList(const std::vector< pozdnyakov::ValueType > &vals)
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

BOOST_AUTO_TEST_CASE(testProcessLogicCorrectness)
{
  pozdnyakov::List< pozdnyakov::NamedSequence > seqs;
  seqs.pushFront({"fourth", makeList({4, 4})});
  seqs.pushFront({"third", makeList({})});
  seqs.pushFront({"second", makeList({2, 2, 2, 2})});
  seqs.pushFront({"first", makeList({1, 1, 1})});

  pozdnyakov::ProcessResult res = pozdnyakov::buildInterleavedRows(seqs);

  std::vector< pozdnyakov::ValueType > expectedSums = {7, 7, 3, 2};
  int i = 0;
  for (auto it = res.sums.begin(); it != res.sums.end(); ++it) {
    BOOST_TEST(*it == expectedSums[i++]);
  }
}

BOOST_AUTO_TEST_CASE(testProcessLogicOverflow)
{
  pozdnyakov::List< pozdnyakov::NamedSequence > seqs;
  seqs.pushFront({"list2", makeList({2})});
  seqs.pushFront({"list1", makeList({18446744073709551615ULL})});

  BOOST_CHECK_THROW(pozdnyakov::buildInterleavedRows(seqs), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
