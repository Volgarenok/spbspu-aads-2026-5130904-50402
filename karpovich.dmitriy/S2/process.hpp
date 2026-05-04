#ifndef PROCESS_HPP
#define PROCESS_HPP
#include <istream>
#include <ostream>
#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace karpovich
{
  void inputQueue(std::istream &in, Queue< Queue< std::string > > &queue);
  void output(std::ostream &out, Stack< long long > res);
}
#endif
