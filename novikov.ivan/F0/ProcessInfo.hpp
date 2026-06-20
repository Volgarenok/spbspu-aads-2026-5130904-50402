#ifndef PROCESS_INFO_HPP
#define PROCESS_INFO_HPP
#include <string>
#include <vector>
#include <windows.h>
#include "AVLTree.hpp"

namespace novikov
{
  struct ProcessDetails {
    DWORD pid;
    std::wstring name;
    std::wstring path;
    double cpuUsage;
    size_t ramUsage;
    bool accessDenied;
  };

  struct WStringLess {
    bool operator()(const std::wstring& lhs, const std::wstring& rhs) const {
      return lhs < rhs;
    }
  };

  struct StringLess {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
      return lhs < rhs;
    }
  };

  using ProcessSnapTree = novikov::AVLTree< std::wstring, std::vector< novikov::ProcessDetails >, novikov::WStringLess >;

  struct Snapshot {
    std::string name;
    novikov::ProcessSnapTree processTree;

    Snapshot* parent = nullptr;
    std::vector< Snapshot* > children;
  };

  using SnapshotTree = novikov::AVLTree< std::string, novikov::Snapshot*, novikov::StringLess >;
}

#endif
