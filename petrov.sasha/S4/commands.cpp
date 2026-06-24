#include "commands.hpp"
#include <cstring>
#include <iostream>

namespace petrov
{
  const Command COMMANDS[] = {
    { "print", printCommand },
    { "complement", complementCommand },
    { "intersect", intersectCommand },
    { "union", unionCommand }
  };
  const std::size_t COMMAND_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);
}

const petrov::Command* petrov::findCommand(const std::string& name)
{
  for (std::size_t i = 0; i < COMMAND_COUNT; ++i) {
    if (name == COMMANDS[i].name) {
      return &COMMANDS[i];
    }
  }
  return nullptr;
}
void petrov::printCommand(Storage& storage, std::istream& in, std::ostream& out)
{
  std::string dataset;
  if (!(in >> dataset) || !storage.has(dataset)) {
    out << "<INVALID COMMAND>" << '\n';
    return;
  }
  const InnerTree& tree = storage.get(dataset);
  if (tree.empty()) {
    out << "<EMPTY>" << '\n';
    return;
  }
  out << dataset;
  for (InnerTree::const_iterator it = tree.begin(); it != tree.end(); ++it) {
    out << ' ' << it->first << ' ' << it->second;
  }
  out << '\n';
}

void petrov::complementCommand(Storage& storage, std::istream& in, std::ostream& out)
{
  std::string newName;
  std::string leftName;
  std::string rightName;
  if (!(in >> newName >> leftName >> rightName) || !storage.has(leftName) || !storage.has(rightName)) {
    out << "<INVALID COMMAND>" << '\n';
    return;
  }
  const InnerTree& left = storage.get(leftName);
  const InnerTree& right = storage.get(rightName);
  InnerTree result;
  for (InnerTree::const_iterator it = left.begin(); it != left.end(); ++it) {
    if (!right.has(it->first)) {
      result.push(it->first, it->second);
    }
  }
  storage.push(newName, result);
}

void petrov::intersectCommand(Storage& storage, std::istream& in, std::ostream& out)
{
  std::string newName;
  std::string leftName;
  std::string rightName;
  if (!(in >> newName >> leftName >> rightName) || !storage.has(leftName) || !storage.has(rightName)) {
    out << "<INVALID COMMAND>" << '\n';
    return;
  }
  const InnerTree& left = storage.get(leftName);
  const InnerTree& right = storage.get(rightName);
  InnerTree result;
  for (InnerTree::const_iterator it = left.begin(); it != left.end(); ++it) {
    if (right.has(it->first)) {
      result.push(it->first, it->second);
    }
  }
  storage.push(newName, result);
}

void petrov::unionCommand(Storage& storage, std::istream& in, std::ostream& out)
{
  std::string newName;
  std::string leftName;
  std::string rightName;
  if (!(in >> newName >> leftName >> rightName) || !storage.has(leftName) || !storage.has(rightName)) {
    out << "<INVALID COMMAND>" << '\n';
    return;
  }
  const InnerTree& left = storage.get(leftName);
  const InnerTree& right = storage.get(rightName);
  InnerTree result;
  for (InnerTree::const_iterator it = left.begin(); it != left.end(); ++it) {
    result.push(it->first, it->second);
  }
  for (InnerTree::const_iterator it = right.begin(); it != right.end(); ++it) {
    if (!result.has(it->first)) {
      result.push(it->first, it->second);
    }
  }
  storage.push(newName, result);
}

