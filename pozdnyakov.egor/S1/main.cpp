#include "list.hpp"
#include "process.hpp"
#include <exception>
#include <iostream>
#include <string>

int main()
{
  try {
    pozdnyakov::List< pozdnyakov::NamedSequence > sequences;
    pozdnyakov::LIter< pozdnyakov::NamedSequence > seqTail = sequences.end();

    std::string name;

    while (std::cin >> name) {
      if (sequences.empty()) {
        sequences.pushFront({name, pozdnyakov::List< pozdnyakov::ValueType >()});
        seqTail = sequences.begin();
      } else {
        sequences.insertAfter(seqTail, {name, pozdnyakov::List< pozdnyakov::ValueType >()});
        ++seqTail;
      }

      pozdnyakov::LIter< pozdnyakov::ValueType > valTail = seqTail->second.end();
      pozdnyakov::ValueType val;

      while (std::cin >> val) {
        if (seqTail->second.empty()) {
          seqTail->second.pushFront(val);
          valTail = seqTail->second.begin();
        } else {
          seqTail->second.insertAfter(valTail, val);
          ++valTail;
        }
      }

      if (std::cin.eof()) {
        break;
      }

      std::cin.clear();
    }

    if (sequences.empty()) {
      std::cout << 0 << "\n";
      return 0;
    }

    bool firstName = true;
    for (auto it = sequences.begin(); it != sequences.end(); ++it) {
      if (!firstName) {
        std::cout << " ";
      }
      std::cout << it->first;
      firstName = false;
    }
    std::cout << "\n";

    pozdnyakov::ProcessResult result = pozdnyakov::buildInterleavedRows(sequences);

    for (auto rowIt = result.rows.begin(); rowIt != result.rows.end(); ++rowIt) {
      bool firstEl = true;
      for (auto elIt = rowIt->begin(); elIt != rowIt->end(); ++elIt) {
        if (!firstEl) {
          std::cout << " ";
        }
        std::cout << *elIt;
        firstEl = false;
      }
      std::cout << "\n";
    }

    bool firstSum = true;
    for (auto sumIt = result.sums.begin(); sumIt != result.sums.end(); ++sumIt) {
      if (!firstSum) {
        std::cout << " ";
      }
      std::cout << *sumIt;
      firstSum = false;
    }
    if (!result.sums.empty()) {
      std::cout << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
