#include "actions.hpp"
#include "queue.hpp"
#include <string>

dirko::Queue< dirko::Queue< std::string > > dirko::input(std::istream &is)
{
  Queue< Queue< std::string > > ret;
  std::string str;
  while (std::getline(is, str)) {
    if (str.empty()) {
      continue;
    }
    Queue< std::string > expretion;
    std::string item;
    for (size_t i = 0; i < str.length(); i++) {
      std::string token = str.substr(i, 1);
      if (token == " ") {
        if (!item.empty()) {
          expretion.push(item);
          item.clear();
        }
      } else {
        item += token;
      }
    }
    if (!item.empty()) {
      expretion.push(item);
    }
    ret.push(expretion);
  }
}
