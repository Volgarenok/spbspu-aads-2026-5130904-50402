#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "List.hpp"
#include <string>
#include <utility>

namespace chernikov {

  using Sequence = std::pair< std::string, List< int > >;

  List< Sequence > parser(std::istream &in);

}

#endif