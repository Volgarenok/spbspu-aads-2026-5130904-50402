#include "list.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <utility>

int main()
{
  using Sequence = std::pair<
    std::string,
    terentev::List< unsigned long long >
  >;
  using SequenceList = terentev::List< Sequence >;
  using SequenceIter = SequenceList::LIter;
  using NumberList = terentev::List< unsigned long long >;
  using NumberIter = NumberList::LIter;

  SequenceList sequences;
  SequenceIter sequencesTail = sequences.begin();
  bool isFirstSequence = true;

  std::string name;
  while (std::cin >> name) {
    NumberList numbers;
    NumberIter numbersTail = numbers.begin();
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

    Sequence element(name, std::move(numbers));

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

  SequenceList tempSequences(sequences);
  NumberList sums;
  NumberIter sumsTail = sums.begin();
  bool isFirstSum = true;

  while (true) {
    bool hasNumbersInThisIteration = false;
    unsigned long long currentSum = 0;

    for (SequenceIter iter = tempSequences.begin();
        iter != tempSequences.end();
        ++iter) {
      NumberList &list = (*iter).second;
      if (!list.isEmpty()) {
        const unsigned long long value = list.front();
        if (currentSum >
            std::numeric_limits< unsigned long long >::max() - value) {
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
  for (SequenceIter iter = sequences.begin();
      iter != sequences.end();
      ++iter) {
    if (!isFirst) {
      std::cout << ' ';
    }
    std::cout << (*iter).first;
    isFirst = false;
  }
  std::cout << '\n';

  SequenceList printSequences(sequences);

  while (true) {
    bool foundInThisLevel = false;
    isFirst = true;

    for (SequenceIter iter = printSequences.begin();
        iter != printSequences.end();
        ++iter) {
      NumberList &list = (*iter).second;
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
    for (NumberIter iter = sums.begin(); iter != sums.end(); ++iter) {
      if (!isFirst) {
        std::cout << ' ';
      }
      std::cout << *iter;
      isFirst = false;
    }
    std::cout << '\n';
  }
}
