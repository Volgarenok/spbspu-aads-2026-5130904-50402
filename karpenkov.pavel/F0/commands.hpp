#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "GraphManager.hpp"
#include <iostream>

namespace karpenkov {
void cmdCreateOrder(std::istream &, std::ostream &, GraphManager &);
void cmdDeleteOrder(std::istream &, std::ostream &, GraphManager &);

void cmdInsertMovie(std::istream &, std::ostream &, GraphManager &);

void cmdShowOrder(std::istream &, std::ostream &, GraphManager &);
void cmdShowDependencies(std::istream &, std::ostream &, GraphManager &);
void cmdShowSuccessors(std::istream &, std::ostream &, GraphManager &);

void cmdFindCycles(std::istream &, std::ostream &, GraphManager &);
void cmdListOrders(std::istream &, std::ostream &, GraphManager &);
void cmdHelp(std::istream &, std::ostream &, GraphManager &);
} // namespace karpenkov

#endif
