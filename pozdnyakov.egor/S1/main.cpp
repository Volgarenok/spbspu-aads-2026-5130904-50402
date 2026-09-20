#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"
#include "process.hpp"

int main()
{
  try {
    pozdnyakov::List< pozdnyakov::NamedSequence > sequences;
    std::string name;

    while (std::cin >> name) {
      pozdnyakov::List< pozdnyakov::ValueType > values;
      pozdnyakov::ValueType value = 0;

      while (std::cin >> value) {
        values.pushBack(value);
      }

      if (!std::cin.eof()) {
        std::cin.clear();
      }

      sequences.pushBack({name, std::move(values)});

      if (std::cin.eof()) {
        break;
      }
    }

    if (sequences.empty()) {
      std::cout << 0 << "\n";
      return 0;
    }

    bool first = true;
    for (auto it = sequences.begin(); it != sequences.end(); ++it) {
      if (!first) {
        std::cout << " ";
      }
      std::cout << it->first;
      first = false;
    }
    std::cout << "\n";

    pozdnyakov::List< pozdnyakov::List< pozdnyakov::ValueType > > rows =
      pozdnyakov::buildInterleavedRows(sequences);

    for (auto rowIt = rows.begin(); rowIt != rows.end(); ++rowIt) {
      bool firstElem = true;
      for (auto elemIt = rowIt->begin(); elemIt != rowIt->end(); ++elemIt) {
        if (!firstElem) {
          std::cout << " ";
        }
        std::cout << *elemIt;
        firstElem = false;
      }
      std::cout << "\n";
    }

    pozdnyakov::List< pozdnyakov::ValueType > sums = pozdnyakov::calculateSums(rows);

    if (sums.empty()) {
      std::cout << 0 << "\n";
    } else {
      bool firstSum = true;
      for (auto sumIt = sums.begin(); sumIt != sums.end(); ++sumIt) {
        if (!firstSum) {
          std::cout << " ";
        }
        std::cout << *sumIt;
        firstSum = false;
      }
      std::cout << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
