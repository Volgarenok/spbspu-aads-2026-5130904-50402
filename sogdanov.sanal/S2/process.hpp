#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <iostream>

#include "stack.hpp"

namespace sogdanov {

  void input(std::istream &in, Stack< long long > &res);
  void output(std::ostream &out, Stack< long long > &res);

}

#endif
