#include "process.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <cstddef>
#include <string>

void karpovich::inputQueue(std::istream &in, Queue< Queue< std::string > > &queue)
{
  std::string s;
  while (std::getline(in, s)) {
    if (s.empty()) {
      continue;
    }
    Queue< std::string > inf;
    for (size_t i = 0; i < s.length(); i++) {
      std::string sChar = s.substr(i, 1);
      if (sChar == " ") {
        continue;
      }
      inf.push(sChar);
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
