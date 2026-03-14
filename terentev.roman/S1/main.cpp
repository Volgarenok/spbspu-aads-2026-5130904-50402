#include "list.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <utility>

int main()
{
  terentev::List< std::pair< std::string, terentev::List< unsigned long long > > > sequences;
  terentev::List< std::pair< std::string, terentev::List< unsigned long long > > >::LIter sequencesTail = sequences.begin();
  bool isFirstSequence = true;

  std::string name;
  while (std::cin >> name) {
    terentev::List< unsigned long long > numbers;
    terentev::List< unsigned long long >::LIter numbersTail = numbers.begin();
    bool isFirstNumber = true;

    while (std::cin.peek() == ' ' || std::cin.peek() == '\t') {
      std::cin.ignore();
    }

    while (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
      unsigned long long value = 0;
      if (!(std::cin >> value)) {
        std::cerr << "invalid input\n";
        return 1;
      }

      if (isFirstNumber) {
        numbers.pushFront(value);
        numbersTail = numbers.begin();
        isFirstNumber = false;
      } else {
        numbers.insertAfter(numbersTail, value);
        ++numbersTail;
      }

      while (std::cin.peek() == ' ' || std::cin.peek() == '\t') {
        std::cin.ignore();
      }
    }

    std::pair< std::string, terentev::List< unsigned long long > > element(name, std::move(numbers));

    if (isFirstSequence) {
      sequences.pushFront(element);
      sequencesTail = sequences.begin();
      isFirstSequence = false;
    } else {
      sequences.insertAfter(sequencesTail, element);
      ++sequencesTail;
    }

    if (std::cin.peek() == '\n') {
      std::cin.ignore();
    }
  }

  if (sequences.isEmpty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  terentev::List< std::pair< std::string, terentev::List< unsigned long long > > > tempSequences(sequences);
  terentev::List< unsigned long long > sums;
  terentev::List< unsigned long long >::LIter sumsTail = sums.begin();
  bool isFirstSum = true;

  while (true) {
    bool hasNumbersInThisIteration = false;
    unsigned long long currentSum = 0;

    for (terentev::List< std::pair< std::string, terentev::List< unsigned long long > > >::LIter iter = tempSequences.begin(); iter != tempSequences.end(); ++iter) {
      terentev::List< unsigned long long > &list = (*iter).second;
      if (!list.isEmpty()) {
        const unsigned long long value = list.front();
        if (currentSum > std::numeric_limits< unsigned long long >::max() - value) {
          std::cerr << "overflow\n";
          return 1;
        }
        currentSum += value;
        list.popFront();
        hasNumbersInThisIteration = true;
      }
    }

    if (!hasNumbersInThisIteration) {
      break;
    }

    if (isFirstSum) {
      sums.pushFront(currentSum);
      sumsTail = sums.begin();
      isFirstSum = false;
    } else {
      sums.insertAfter(sumsTail, currentSum);
      ++sumsTail;
    }
  }

  bool isFirst = true;
  for (terentev::List< std::pair< std::string, terentev::List< unsigned long long > > >::LIter iter = sequences.begin(); iter != sequences.end(); ++iter) {
    if (!isFirst) {
      std::cout << ' ';
    }
    std::cout << (*iter).first;
    isFirst = false;
  }
  std::cout << '\n';

  terentev::List< std::pair< std::string, terentev::List< unsigned long long > > > printSequences(sequences);

  while (true) {
    bool foundInThisLevel = false;
    isFirst = true;

    for (terentev::List< std::pair< std::string, terentev::List< unsigned long long > > >::LIter iter = printSequences.begin(); iter != printSequences.end(); ++iter) {
      terentev::List< unsigned long long > &list = (*iter).second;
      if (!list.isEmpty()) {
        if (!isFirst) {
          std::cout << ' ';
        }
        std::cout << list.front();
        list.popFront();
        foundInThisLevel = true;
        isFirst = false;
      }
    }

    if (!foundInThisLevel) {
      break;
    }
    std::cout << '\n';
  }

  if (sums.isEmpty()) {
    std::cout << 0 << '\n';
  } else {
    isFirst = true;
    for (terentev::List< unsigned long long >::LIter iter = sums.begin(); iter != sums.end(); ++iter) {
      if (!isFirst) {
        std::cout << ' ';
      }
      std::cout << *iter;
      isFirst = false;
    }
    std::cout << '\n';
  }
}
