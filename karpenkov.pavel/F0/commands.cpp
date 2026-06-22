#include "commands.hpp"

namespace karpenkov {
void cmdCreateOrder(std::istream &in, std::ostream &out,
                    GraphManager &manager) {
  std::string name;
  in >> name;
  manager.createOrder(name);
  out << "Order \"" << name << "\" was created\n";
}

void cmdDeleteOrder(std::istream &in, std::ostream &out,
                    GraphManager &manager) {
  std::string name;
  in >> name;
  manager.deleteOrder(name);
  out << "Order \"" << name << "\" was deleted\n";
}

void cmdInsertMovie(std::istream &in, std::ostream &out,
                    GraphManager &manager) {
  std::string order;
  std::string before;
  std::string after;
  in >> order >> before >> after;
  manager.getOrder(order).insertMovie(before, after);
  out << before << " inserted before " << after << '\n';
}

void cmdShowOrder(std::istream &in, std::ostream &out, GraphManager &manager) {
  std::string order;
  in >> order;
  manager.getOrder(order).showOrder(out);
}

void cmdShowDependencies(std::istream &in, std::ostream &out,
                         GraphManager &manager) {
  std::string order;
  std::string movie;
  in >> order >> movie;
  manager.getOrder(order).showDependencies(movie, out);
}

void cmdShowSuccessors(std::istream &in, std::ostream &out,
                       GraphManager &manager) {
  std::string order;
  std::string movie;
  in >> order >> movie;
  manager.getOrder(order).showSuccessors(movie, out);
}

void cmdFindCycles(std::istream &in, std::ostream &out, GraphManager &manager) {
  std::string order;
  in >> order;
  manager.getOrder(order).findCycles(out);
}
void cmdListOrders(std::istream &, std::ostream &out, GraphManager &manager) {
  manager.listOrders(out);
}
void cmdHelp(std::istream &, std::ostream &out, GraphManager &) {
  out << "Available commands:\n";
  out << "  help\n";
  out << "  create-order <graph>\n";
  out << "  delete-order <graph>\n";
  out << "  list-orders\n";
  out << "  insert-movie <graph> <movie> <dependency>\n";
  out << "  show-order <graph>\n";
  out << "  show-dependencies <graph> <movie>\n";
  out << "  show-successors <graph> <movie>\n";
  out << "  find-cycles <graph>\n";
}
} // namespace karpenkov
