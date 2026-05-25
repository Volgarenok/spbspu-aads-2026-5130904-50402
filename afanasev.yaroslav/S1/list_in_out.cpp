#include "list_in_out.hpp"
#include <limits>

namespace afanasev
{
  namespace
  {
    void outputNumbers(std::ostream & out, size_t maxLen, List< LIter< size_t > > numbers)
    {
      if (!maxLen)
      {
        out << "0\n";
        return;
      }

      LIter< size_t > numEnd(nullptr);
      List< size_t > sums;
      LIter< size_t > lastSum(nullptr);
      bool firstSum = true;
      bool overflow = false;

      for (size_t row = 0; row < maxLen; ++row)
      {
        size_t rowSum = 0;
        LIter< LIter< size_t > > numsEnd(nullptr);
        bool firstCol = true;

        for (LIter< LIter< size_t > > numIt = numbers.begin(); numIt != numsEnd; ++numIt)
        {
          LIter< size_t > & current = * numIt;

          if (current != numEnd)
          {
            if (!firstCol)
            {
              out << ' ';
            }
            else
            {
              firstCol = false;
            }

            out << * current;

            if (rowSum > std::numeric_limits< size_t >::max() - * current)
            {
              overflow = true;
            }
            else
            {
              rowSum += * current;
            }
            ++current;
          }
        }
        out << '\n';

        if (firstSum)
        {
          sums.pushFront(rowSum);
          lastSum = sums.begin();
          firstSum = false;
        }
        else
        {
          sums.insert(rowSum, lastSum);
          ++lastSum;
        }
      }

      if (overflow)
      {
        throw std::overflow_error("Overflow");
      }

      LIter< size_t > sumsEnd(nullptr);

      for (LIter< size_t > sIt = sums.begin(); sIt != sumsEnd; ++sIt)
      {
        if (sIt != sums.begin())
        {
          out << ' ';
        }
        out << * sIt;
      }
      out << '\n';
    }
  }

  bool input(std::istream & in, List< std::pair< std::string, List< size_t > > > & list)
  {
    std::string name;

    if (!(in >> name))
    {
      return false;
    }

    List< size_t > numbers;
    size_t num;

    if (in >> num)
    {
      numbers.pushFront(num);
      LIter< size_t > lastNum = numbers.begin();

      while (in >> num)
      {
        numbers.insert(num, lastNum);
        ++lastNum;
      }
    }

    in.clear();
    list.pushFront({name, numbers});

    LIter< std::pair< std::string, List< size_t > > > lastPair = list.begin();

    while (in >> name)
    {
      List< size_t > numbers;
      size_t num;

      if (in >> num)
      {
        numbers.pushFront(num);
        LIter< size_t > lastNum = numbers.begin();

        while (in >> num)
        {
          numbers.insert(num, lastNum);
          ++lastNum;
        }
      }

      in.clear();
      list.insert({name, numbers}, lastPair);
      ++lastPair;
    }
    return true;
  }

  void output(std::ostream & out, List< std::pair< std::string, List< size_t > > > & list)
  {
    LIter< std::pair< std::string, List< size_t > > > end(nullptr);
    size_t lenght = 0;

    List< LIter< size_t > > numbers;
    size_t maxLen = 0;
    LIter< LIter< size_t > > lastNumIter(nullptr);
    bool firstNum = true;

    for (LIter< std::pair< std::string, List< size_t > > > it = list.begin(); it != end; ++it)
    {
      if (lenght)
      {
        out << ' ' << (* it).first;
        ++lenght;
      }
      else
      {
        out << (* it).first;
        ++lenght;
      }

      List< size_t > & numList = (* it).second;
      size_t sz = numList.size();
      maxLen = std::max(sz, maxLen);

      LIter< size_t > beginNum = numList.begin();

      if (!firstNum)
      {
        numbers.insert(beginNum, lastNumIter);
        ++lastNumIter;
      }
      else
      {
        numbers.pushFront(beginNum);
        lastNumIter = numbers.begin();
        firstNum = false;
      }
    }
    out << '\n';

    outputNumbers(out, maxLen, numbers);
  }
}
