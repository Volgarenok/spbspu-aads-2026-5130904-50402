#include "process.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <cstddef>
#include <string>

void karpovich::inputQueue(std::istream &in, Queue< Queue< std::string > > &queue)
{
  std::string str;
  while (std::getline(in, str)) {
    if (str.empty()) {
      continue;
    }
    Queue< std::string > inf;
    std::string token;
    for (size_t i = 0; i < str.length(); i++) {
      std::string sChar = str.substr(i, 1);
      if (sChar == " ") {
        if (!token.empty()) {
          inf.push(token);
          token.clear();
        }
      } else {
        token += sChar;
      }
    }
    if (!token.empty()) {
      inf.push(token);
    }
    queue.push(inf);
  }
}
void karpovich::output(std::ostream &out, Stack< long long > res)
{
  if (!res.empty()) {
    out << res.top();
    res.pop();
  }
  while (!res.empty()) {
    out << ' ';
    out << res.top();
    res.pop();
  }
  out << '\n';
}
