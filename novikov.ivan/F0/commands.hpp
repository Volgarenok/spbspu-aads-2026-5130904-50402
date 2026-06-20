#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "ProcessInfo.hpp"

namespace novikov
{
  void snap(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void displayAllProcesses(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void getProcessDetails(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void compareSnapshots(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void filterRAM(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void filterCPU(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void exportSnapshot(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void deleteSnapshot(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void list(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void clearAll(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
  void help(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps);
}

#endif
