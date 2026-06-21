#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <sstream>
#include <string>
#include <utility>
#include <functional>
#include <iostream>
#include "matrix.hpp"
#include "RBTree.hpp"

using pair_t = std::pair< std::string, khalikov::Matrix >;
using Storage = khalikov::RBTree< pair_t, std::function< bool(const pair_t &, const pair_t &) > >;
using cmd_t = void (*)(std::ostream &, std::istream &, Storage &);

namespace khalikov
{
  const khalikov::Matrix *find(const Storage &storage, const std::string &name);
  khalikov::Matrix *find(Storage &storage, const std::string &name);
  void checkInput(std::istream &in);

  void create(std::ostream &, std::istream &in, Storage &storage);
  void fill(std::ostream &, std::istream &in, Storage &storage);
  void show(std::ostream &out, std::istream &in, Storage &storage);
  void add(std::ostream &, std::istream &in, Storage &storage);
  void mul(std::ostream &, std::istream &in, Storage &storage);
  void sub(std::ostream &, std::istream &in, Storage &storage);
  void remove(std::ostream &out, std::istream &in, Storage &storage);
  void clear(std::ostream &, std::istream &in, Storage &storage);
  void list(std::ostream &out, std::istream &in, Storage &storage);
  void basech(std::ostream &, std::istream &in, Storage &storage);
  void transpose(std::ostream &, std::istream &in, Storage &storage);
  void scale(std::ostream &, std::istream &in, Storage &storage);
  void det(std::ostream &, std::istream &in, Storage &storage);
  void eye(std::ostream &, std::istream &in, Storage &storage);
  void rotateR(std::ostream &, std::istream &in, Storage &storage);
  void rotateL(std::ostream &, std::istream &in, Storage &storage);
  void square(std::ostream &, std::istream &in, Storage &storage);
  void mergeV(std::ostream &, std::istream &in, Storage &storage);
  void mergeH(std::ostream &, std::istream &in, Storage &storage);
  void swapC(std::ostream &, std::istream &in, Storage &storage);
  void swapR(std::ostream &, std::istream &in, Storage &storage);
  void splitH(std::ostream &, std::istream &in, Storage &storage);
  void splitV(std::ostream &, std::istream &in, Storage &storage);
  void reshape(std::ostream &, std::istream &in, Storage &storage);
}

#endif
