#include "process.hpp"
#include <stdexcept>
#include <limits>

namespace pozdnyakov
{

  ProcessResult buildInterleavedRows(const List< NamedSequence > &sequences)
  {
    ProcessResult result;

    List< LCIter< ValueType > > iters;
    List< LCIter< ValueType > > ends;

    LIter< LCIter< ValueType > > itersTail = iters.end();
    LIter< LCIter< ValueType > > endsTail = ends.end();

    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
      if (iters.empty()) {
        iters.pushFront((*it).second.cbegin());
        ends.pushFront((*it).second.cend());
        itersTail = iters.begin();
        endsTail = ends.begin();
      } else {
        iters.insertAfter(itersTail, (*it).second.cbegin());
        ends.insertAfter(endsTail, (*it).second.cend());
        ++itersTail;
        ++endsTail;
      }
    }

    LIter< List< ValueType > > rowsTail = result.rows.end();
    LIter< ValueType > sumsTail = result.sums.end();

    while (true) {
      bool elementsLeft = false;
      ValueType currentRowSum = 0;

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

          if (currentRowSum > std::numeric_limits< ValueType >::max() - val) {
            throw std::overflow_error("Sum calculation overflow");
          }
          currentRowSum += val;
          ++(*it);
        }
      }

      if (!elementsLeft)
        break;

      if (result.rows.empty()) {
        result.rows.pushFront(currentRow);
        rowsTail = result.rows.begin();
      } else {
        result.rows.insertAfter(rowsTail, currentRow);
        ++rowsTail;
      }

      if (result.sums.empty()) {
        result.sums.pushFront(currentRowSum);
        sumsTail = result.sums.begin();
      } else {
        result.sums.insertAfter(sumsTail, currentRowSum);
        ++sumsTail;
      }
    }

    return result;
  }

}
