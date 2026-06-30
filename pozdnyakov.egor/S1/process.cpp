#include "process.hpp"
#include <limits>
#include <stdexcept>

namespace pozdnyakov
{

  List< List< ValueType > > buildInterleavedRows(List< NamedSequence > &sequences)
  {
    List< LIter< ValueType > > iterators;
    List< LIter< ValueType > > endIterators;

    for (auto it = sequences.begin(); it != sequences.end(); ++it) {
      iterators.pushBack(it->second.begin());
      endIterators.pushBack(it->second.end());
    }

    List< List< ValueType > > rows;

    while (true) {
      bool elementsLeft = false;
      List< ValueType > row;

      auto it = iterators.begin();
      auto endIt = endIterators.begin();

      for (; it != iterators.end() && endIt != endIterators.end(); ++it, ++endIt) {
        if (*it != *endIt) {
          elementsLeft = true;
          row.pushBack(*(*it));
          ++(*it);
        }
      }

      if (!elementsLeft) {
        break;
      }

      rows.pushBack(std::move(row));
    }

    return rows;
  }

  List< ValueType > calculateSums(List< List< ValueType > > &rows)
  {
    List< ValueType > sums;

    for (auto rowIt = rows.begin(); rowIt != rows.end(); ++rowIt) {
      ValueType sum = 0;

      for (auto elemIt = rowIt->begin(); elemIt != rowIt->end(); ++elemIt) {
        const ValueType value = *elemIt;
        if (sum > std::numeric_limits< ValueType >::max() - value) {
          throw std::overflow_error("Sum calculation overflow");
        }
        sum += value;
      }

      sums.pushBack(sum);
    }

    return sums;
  }

}
