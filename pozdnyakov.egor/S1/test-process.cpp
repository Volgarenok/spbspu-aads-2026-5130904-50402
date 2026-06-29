#include <cstddef>
#include <limits>
#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "list.hpp"
#include "process.hpp"

using namespace pozdnyakov;

namespace
{

  List< ValueType > makeList(const ValueType *values, std::size_t size)
  {
    List< ValueType > list;
    for (std::size_t i = size; i > 0; --i) {
      list.pushFront(values[i - 1]);
    }
    return list;
  }

}

BOOST_AUTO_TEST_SUITE(ProcessTests)

BOOST_AUTO_TEST_CASE(testEmptySequences)
{
  List< NamedSequence > sequences;

  List< List< ValueType > > rows = buildInterleavedRows(sequences);
  List< ValueType > sums = calculateSums(rows);

  BOOST_CHECK(rows.empty());
  BOOST_CHECK(sums.empty());
}

BOOST_AUTO_TEST_CASE(testSequencesWithoutNumbers)
{
  List< NamedSequence > sequences;
  sequences.pushFront({"second", List< ValueType >()});
  sequences.pushFront({"first", List< ValueType >()});

  List< List< ValueType > > rows = buildInterleavedRows(sequences);
  List< ValueType > sums = calculateSums(rows);

  BOOST_CHECK(rows.empty());
  BOOST_CHECK(sums.empty());
}

BOOST_AUTO_TEST_CASE(testSingleSequence)
{
  List< NamedSequence > sequences;
  const ValueType values[] = {10, 20};
  sequences.pushFront({"single", makeList(values, 2)});

  List< List< ValueType > > rows = buildInterleavedRows(sequences);
  List< ValueType > sums = calculateSums(rows);

  const ValueType expectedSums[] = {10, 20};
  int i = 0;
  for (auto it = sums.begin(); it != sums.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expectedSums[i++]);
  }
  BOOST_CHECK_EQUAL(i, 2);
}

BOOST_AUTO_TEST_CASE(testProcessLogicOverflow)
{
  List< NamedSequence > sequences;

  const ValueType value2[] = {2};
  sequences.pushFront({"list2", makeList(value2, 1)});

  const ValueType value1[] = {std::numeric_limits< ValueType >::max()};
  sequences.pushFront({"list1", makeList(value1, 1)});

  List< List< ValueType > > rows = buildInterleavedRows(sequences);
  BOOST_CHECK_THROW(calculateSums(rows), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(testAssignmentExample)
{
  List< NamedSequence > sequences;

  const ValueType vFirst[] = {1, 1, 1};
  const ValueType vSecond[] = {2, 2, 2, 2};
  const ValueType vFourth[] = {4, 4};

  sequences.pushBack({"first", makeList(vFirst, 3)});
  sequences.pushBack({"second", makeList(vSecond, 4)});
  sequences.pushBack({"third", List< ValueType >()});
  sequences.pushBack({"fourth", makeList(vFourth, 2)});

  List< List< ValueType > > rows = buildInterleavedRows(sequences);

  const ValueType expectedRows[4][3] = {
    {1, 2, 4},
    {1, 2, 4},
    {1, 2, 0},
    {2, 0, 0}
  };
  const size_t expectedRowSizes[] = {3, 3, 2, 1};

  size_t rowIdx = 0;
  for (auto rowIt = rows.begin(); rowIt != rows.end(); ++rowIt) {
    size_t elemIdx = 0;
    for (auto elemIt = rowIt->begin(); elemIt != rowIt->end(); ++elemIt) {
      BOOST_CHECK_EQUAL(*elemIt, expectedRows[rowIdx][elemIdx]);
      ++elemIdx;
    }
    BOOST_CHECK_EQUAL(elemIdx, expectedRowSizes[rowIdx]);
    ++rowIdx;
  }
  BOOST_CHECK_EQUAL(rowIdx, 4u);

  List< ValueType > sums = calculateSums(rows);
  const ValueType expectedSums[] = {7, 7, 3, 2};
  size_t i = 0;
  for (auto it = sums.begin(); it != sums.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expectedSums[i++]);
  }
  BOOST_CHECK_EQUAL(i, 4u);
}

BOOST_AUTO_TEST_CASE(testEqualLengthSequences)
{
  List< NamedSequence > sequences;

  const ValueType vA[] = {1, 2, 3};
  const ValueType vB[] = {4, 5, 6};
  sequences.pushBack({"a", makeList(vA, 3)});
  sequences.pushBack({"b", makeList(vB, 3)});

  List< List< ValueType > > rows = buildInterleavedRows(sequences);
  List< ValueType > sums = calculateSums(rows);

  const ValueType expectedSums[] = {5, 7, 9};
  size_t i = 0;
  for (auto it = sums.begin(); it != sums.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expectedSums[i++]);
  }
  BOOST_CHECK_EQUAL(i, 3u);
}

BOOST_AUTO_TEST_CASE(testRowsAreNotEmpty)
{
  List< NamedSequence > sequences;

  const ValueType vX[] = {5, 10, 15};
  sequences.pushBack({"x", makeList(vX, 3)});

  List< List< ValueType > > rows = buildInterleavedRows(sequences);
  BOOST_CHECK(!rows.empty());

  size_t rowCount = 0;
  for (auto it = rows.begin(); it != rows.end(); ++it) {
    ++rowCount;
  }
  BOOST_CHECK_EQUAL(rowCount, 3u);
}

BOOST_AUTO_TEST_SUITE_END()
