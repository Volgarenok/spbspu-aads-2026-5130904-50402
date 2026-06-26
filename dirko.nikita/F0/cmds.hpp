#ifndef CMDS_HPP
#define CMDS_HPP
#include <iosfwd>
#include "player.hpp"
namespace dirko
{
  using cmd_t = void (*)(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void play(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void next(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void prev(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void add(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void remove(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void rename(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void loop(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void random(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void list(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_add(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_remove(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_rename(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_add_track(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_remove_track(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_get(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_list(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_merge(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_select(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
  void pl_diff(std::istream &, std::ostream &, pls_t &, pl_iter_t &, pls_iter_t &, save_t &);
}
#endif
