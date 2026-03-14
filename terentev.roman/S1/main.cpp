#include "list.hpp"
#include <iostream>

int main()
{
  terentev::List< std::pair< std::string, terentev::List< int > > > sequences;
  terentev::List< std::pair< std::string, terentev::List< int > > >::LIter
      sequencesTail;
  bool isFirstSequence = true;

  std::string name;
  while (std::cin >> name) {
    terentev::List< int > numbers;
    terentev::List< int >::LIter numbersTail;
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

      int value = 0;
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

    std::pair< std::string, terentev::List< int > > element(
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
  terentev::List< std::pair< std::string, terentev::List< int > > >::LCIter
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

  terentev::List< size_t > sums;
  terentev::List< size_t >::LIter sumsTail;
  bool isFirstSum = true;

  while (true) {
    bool hasNumbers = false;
    bool isFirstOutputNumber = true;
    size_t sum = 0;

    terentev::List< std::pair< std::string, terentev::List< int > > >::LIter
        iter = sequences.begin();

    while (iter != sequences.end()) {
      terentev::List< int > &currentList = (*iter).second;

      if (!currentList.isEmpty()) {
        int value = currentList.front();

        if (value < 0) {
          std::cerr << "Invalid input\n";
          return 1;
        }

        if (sum > static_cast< size_t >(-1) - static_cast< size_t >(value)) {
          std::cerr << "Sum overflow\n";
          return 1;
        }

        if (!isFirstOutputNumber) {
          std::cout << ' ';
        }
        std::cout << value;

        sum += static_cast< size_t >(value);
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

  bool isFirstOutputSum = true;
  terentev::List< size_t >::LCIter sumsIter = sums.begin();
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
