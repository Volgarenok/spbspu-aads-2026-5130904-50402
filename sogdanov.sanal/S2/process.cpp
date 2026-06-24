#include "process.hpp"

#include <string>

#include "operators.hpp"

void sogdanov::input(std::istream &in, sogdanov::Stack< long long > &res)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    res.push(sogdanov::evaluate(line));
  }
}

void sogdanov::output(std::ostream &out, sogdanov::Stack< long long > &res)
{
  bool first = true;
  while (!res.empty()) {
    if (!first) {
      out << ' ';
    }
    out << res.top();
    res.pop();
    first = false;
  }
  out << '\n';
}
