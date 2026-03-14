#include "list.hpp"
#include <climits>
#include <iostream>
#include <utility>

int main()
{
  terentev::List< std::pair< std::string, terentev::List< unsigned long long > > >
      sequences;
  terentev::List<
      std::pair< std::string, terentev::List< unsigned long long > > >::LIter
      sequencesTail = sequences.begin();
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
        std::cerr << "Invalid input\n";
        return 2;
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

    std::pair< std::string, terentev::List< unsigned long long > > element(
        name,
        std::move(numbers));

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

  terentev::List< std::pair< std::string, terentev::List< unsigned long long > > >
      tempSequences(sequences);
  terentev::List< unsigned long long > sums;
  terentev::List< unsigned long long >::LIter sumsTail = sums.begin();
  bool isFirstSum = true;

  while (true) {
    bool hasNumbers = false;
    unsigned long long sum = 0;

    terentev::List<
        std::pair< std::string, terentev::List< unsigned long long > > >::LIter
        iter = tempSequences.begin();
    while (iter != tempSequences.end()) {
      terentev::List< unsigned long long > &currentList = (*iter).second;
      if (!currentList.isEmpty()) {
        const unsigned long long value = currentList.front();
        if (sum > ULLONG_MAX - value) {
          std::cerr << "Overflow\n";
          return 2;
        }
        sum += value;
        currentList.popFront();
        hasNumbers = true;
      }
      ++iter;
    }

    if (!hasNumbers) {
      break;
    }

    if (isFirstSum) {
      sums.pushFront(sum);
      sumsTail = sums.begin();
      isFirstSum = false;
    } else {
      sums.insertAfter(sumsTail, sum);
      ++sumsTail;
    }
  }

  bool isFirstName = true;
  for (terentev::List<
      std::pair< std::string, terentev::List< unsigned long long > > >::LIter
      iter = sequences.begin(); iter != sequences.end(); ++iter) {
    if (!isFirstName) {
      std::cout << ' ';
    }
    std::cout << (*iter).first;
    isFirstName = false;
  }
  std::cout << '\n';

  while (true) {
    bool hasNumbers = false;
    bool isFirstInRow = true;

    for (terentev::List<
        std::pair< std::string, terentev::List< unsigned long long > > >::LIter
        iter = sequences.begin(); iter != sequences.end(); ++iter) {
      terentev::List< unsigned long long > &currentList = (*iter).second;
      if (!currentList.isEmpty()) {
        if (!isFirstInRow) {
          std::cout << ' ';
        }
        std::cout << currentList.front();
        currentList.popFront();
        hasNumbers = true;
        isFirstInRow = false;
      }
    }

    if (!hasNumbers) {
      break;
    }
    std::cout << '\n';
  }

  bool isFirstOutputSum = true;
  for (terentev::List< unsigned long long >::LIter iter = sums.begin();
      iter != sums.end(); ++iter) {
    if (!isFirstOutputSum) {
      std::cout << ' ';
    }
    std::cout << *iter;
    isFirstOutputSum = false;
  }
  std::cout << '\n';
}
