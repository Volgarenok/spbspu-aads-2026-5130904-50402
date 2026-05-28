#ifndef PROCESS_HPP
#define PROCESS_HPP
#include <istream>
#include <ostream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace karpovich
{
  void inputQueue(std::istream &in, Queue< Queue< std::string > > &queue);
  void output(std::ostream &out, Stack< long long > res);
}
#endif
