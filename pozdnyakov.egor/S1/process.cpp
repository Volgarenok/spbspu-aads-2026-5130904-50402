#include "process.hpp"
#include <limits>
#include <stdexcept>

namespace pozdnyakov
{

  List< List< ValueType > > buildInterleavedRows(const List< NamedSequence > &sequences)
  {
    List< List< ValueType > > result;

    List< LCIter< ValueType > > iters;
    List< LCIter< ValueType > > ends;

    LIter< LCIter< ValueType > > itersTail = iters.end();
    LIter< LCIter< ValueType > > endsTail = ends.end();

    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
      if (iters.empty()) {
        iters.pushFront(it->second.cbegin());
        ends.pushFront(it->second.cend());
        itersTail = iters.begin();
        endsTail = ends.begin();
      } else {
        iters.insertAfter(itersTail, it->second.cbegin());
        ends.insertAfter(endsTail, it->second.cend());
        ++itersTail;
        ++endsTail;
      }
    }

    LIter< List< ValueType > > rowsTail = result.end();

    while (true) {
      bool elementsLeft = false;

      List< ValueType > currentRow;
      LIter< ValueType > currentRowTail = currentRow.end();

      auto it = iters.begin();
      auto endIt = ends.begin();

      for (; it != iters.end() && endIt != ends.end(); ++it, ++endIt) {
        if (*it != *endIt) {
          elementsLeft = true;
          ValueType val = *(*it);

          if (currentRow.empty()) {
            currentRow.pushFront(val);
            currentRowTail = currentRow.begin();
          } else {
            currentRow.insertAfter(currentRowTail, val);
            ++currentRowTail;
          }

          ++(*it);
        }
      }

      if (!elementsLeft) {
        break;
      }

      if (result.empty()) {
        result.pushFront(currentRow);
        rowsTail = result.begin();
      } else {
        result.insertAfter(rowsTail, currentRow);
        ++rowsTail;
      }
    }

    return result;
  }

  List< ValueType > calculateSums(const List< List< ValueType > > &rows)
  {
    List< ValueType > sums;
    LIter< ValueType > sumsTail = sums.end();

    for (auto rowIt = rows.cbegin(); rowIt != rows.cend(); ++rowIt) {
      ValueType currentRowSum = 0;

      for (auto elIt = rowIt->cbegin(); elIt != rowIt->cend(); ++elIt) {
        if (currentRowSum > std::numeric_limits< ValueType >::max() - *elIt) {
          throw std::overflow_error("Sum calculation overflow");
        }
        currentRowSum += *elIt;
      }

      if (sums.empty()) {
        sums.pushFront(currentRowSum);
        sumsTail = sums.begin();
      } else {
        sums.insertAfter(sumsTail, currentRowSum);
        ++sumsTail;
      }
    }

    return sums;
  }

}
