#ifndef PROCESS_UTILITIES_HPP
#define PROCESS_UTILITIES_HPP
#include <string>
#include "ProcessInfo.hpp"

namespace novikov
{
  std::string toUtf8(const std::wstring& wstr);
  std::wstring toWString(const std::string& str);
  bool fileExists(const std::string& filename);
  void freeSnapshotSubtree(novikov::Snapshot* snap, novikov::SnapshotTree& snaps);
  void takeSystemSnapshot(novikov::ProcessSnapTree& tree);
}

#endif
