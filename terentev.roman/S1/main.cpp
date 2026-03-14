#include "list.hpp"
#include <climits>
#include <iostream>
#include <string>
#include <utility>

int main()
{
  terentev::List< std::pair< std::string, terentev::List< unsigned long long > > >
      sequences;
  terentev::List<
      std::pair< std::string, terentev::List< unsigned long long > > >::LIter
      sequencesTail;
  bool isFirstSequence = true;

  std::string name;
  while (std::cin >> name) {
    terentev::List< unsigned long long > numbers;
    terentev::List< unsigned long long >::LIter numbersTail;
    bool isFirstNumber = true;

    char symbol = '\0';
    while (std::cin.get(symbol)) {
      if (symbol == '\n') {
        break;
      }
      if (symbol == ' ' || symbol == '\t' || symbol == '\r') {
        continue;
      }

      std::cin.unget();

      unsigned long long value = 0;
      if (!(std::cin >> value)) {
        std::cerr << "Invalid input\n";
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
  }

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Invalid input\n";
    return 1;
  }

  if (sequences.isEmpty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  bool isFirstName = true;
  terentev::List<
      std::pair< std::string, terentev::List< unsigned long long > > >::LCIter
      sequenceIter = sequences.begin();
  while (sequenceIter != sequences.end()) {
    if (!isFirstName) {
      std::cout << ' ';
    }
    std::cout << (*sequenceIter).first;
    isFirstName = false;
    ++sequenceIter;
  }
  std::cout << '\n';

  terentev::List< unsigned long long > sums;
  terentev::List< unsigned long long >::LIter sumsTail;
  bool isFirstSum = true;

  while (true) {
    bool hasNumbers = false;
    bool isFirstOutputNumber = true;
    unsigned long long sum = 0;

    terentev::List<
        std::pair< std::string, terentev::List< unsigned long long > > >::LIter
        iter = sequences.begin();

    while (iter != sequences.end()) {
      terentev::List< unsigned long long > &currentList = (*iter).second;

      if (!currentList.isEmpty()) {
        unsigned long long value = currentList.front();

        if (sum > ULLONG_MAX - value) {
          std::cerr << "Overflow\n";
          return 1;
        }

        if (!isFirstOutputNumber) {
          std::cout << ' ';
        }
        std::cout << value;

        sum += value;
        currentList.popFront();

        hasNumbers = true;
        isFirstOutputNumber = false;
      }

      ++iter;
    }

    if (!hasNumbers) {
      break;
    }

    std::cout << '\n';

    if (isFirstSum) {
      sums.pushFront(sum);
      sumsTail = sums.begin();
      isFirstSum = false;
    } else {
      sums.insertAfter(sumsTail, sum);
      ++sumsTail;
    }
  }

  if (sums.isEmpty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  bool isFirstOutputSum = true;
  terentev::List< unsigned long long >::LCIter sumsIter = sums.begin();
  while (sumsIter != sums.end()) {
    if (!isFirstOutputSum) {
      std::cout << ' ';
    }
    std::cout << *sumsIter;
    isFirstOutputSum = false;
    ++sumsIter;
  }
  std::cout << '\n';
}
