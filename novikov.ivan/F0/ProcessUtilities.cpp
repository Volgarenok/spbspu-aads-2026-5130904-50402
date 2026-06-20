#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <fstream>
#include <algorithm>
#include <map>
#include <cmath>
#include <vector>
#include "ProcessUtilities.hpp"

namespace novikov
{
  std::string toUtf8(const std::wstring& wstr)
  {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
  }

  std::wstring toWString(const std::string& str)
  {
    if (str.empty()) return L"";
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
  }

  bool fileExists(const std::string& filename)
  {
    std::ifstream f(filename.c_str());
    return f.good();
  }

  void freeSnapshotSubtree(novikov::Snapshot* snap, novikov::SnapshotTree& snaps)
  {
    if (!snap) return;

    for (Snapshot* child : snap->children)
    {
      freeSnapshotSubtree(child, snaps);
    }

    if (snaps.has(snap->name))
    {
      snaps.drop(snap->name);
    }

    delete snap;
  }

  void takeSystemSnapshot(novikov::ProcessSnapTree& tree)
  {
    struct TempCpuTime {
      std::wstring name;
      unsigned long long kernelTime;
      unsigned long long userTime;
    };

    std::map< DWORD, TempCpuTime > firstPassData;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32))
    {
      do
      {
        DWORD pid = pe32.th32ProcessID;
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (hProc != NULL)
        {
          FILETIME ftCreate, ftExit, ftKernel, ftUser;
          if (GetProcessTimes(hProc, &ftCreate, &ftExit, &ftKernel, &ftUser))
          {
            ULARGE_INTEGER k, u;
            k.LowPart = ftKernel.dwLowDateTime;
            k.HighPart = ftKernel.dwHighDateTime;
            u.LowPart = ftUser.dwLowDateTime;
            u.HighPart = ftUser.dwHighDateTime;

            firstPassData[pid] = TempCpuTime{ pe32.szExeFile, k.QuadPart, u.QuadPart };
          }
          CloseHandle(hProc);
        }
      } while (Process32NextW(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);

    Sleep(100);

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD numProcessors = sysInfo.dwNumberOfProcessors;

    if (Process32FirstW(hSnapshot, &pe32))
    {
      do
      {
        DWORD pid = pe32.th32ProcessID;
        ProcessDetails details;
        details.pid = pid;
        details.name = pe32.szExeFile;

        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProc != NULL)
        {
          details.accessDenied = false;

          wchar_t exePath[MAX_PATH];
          DWORD size = MAX_PATH;
          if (QueryFullProcessImageNameW(hProc, 0, exePath, &size))
          {
            details.path = exePath;
          }
          else
          {
            details.path = L"N/A";
          }

          PROCESS_MEMORY_COUNTERS pmc;
          if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc)))
          {
            details.ramUsage = pmc.WorkingSetSize / (1024 * 1024);
          }
          else
          {
            details.ramUsage = 0;
          }

          if (firstPassData.find(pid) != firstPassData.end())
          {
            FILETIME ftCreate, ftExit, ftKernel, ftUser;
            if (GetProcessTimes(hProc, &ftCreate, &ftExit, &ftKernel, &ftUser))
            {
              ULARGE_INTEGER k2, u2;
              k2.LowPart = ftKernel.dwLowDateTime;
              k2.HighPart = ftKernel.dwHighDateTime;
              u2.LowPart = ftUser.dwLowDateTime;
              u2.HighPart = ftUser.dwHighDateTime;

              unsigned long long k_delta = k2.QuadPart - firstPassData[pid].kernelTime;
              unsigned long long u_delta = u2.QuadPart - firstPassData[pid].userTime;
              unsigned long long total_delta = k_delta + u_delta;

              double rawCpu = (double)total_delta / 10000.0 / numProcessors;

              details.cpuUsage = std::round(rawCpu * 10.0) / 10.0;

              if (details.cpuUsage > 100.0) details.cpuUsage = 100.0;
            }
            else
            {
              details.cpuUsage = 0.0;
            }
          }
          else
          {
            details.cpuUsage = 0.0;
          }

          CloseHandle(hProc);
        }
        else
        {
          details.path = L"N/A";
          details.ramUsage = 0;
          details.cpuUsage = 0.0;
          details.accessDenied = true;
        }

        if (tree.has(details.name))
        {
          tree[details.name].push_back(details);
        }
        else
        {
          std::vector<ProcessDetails> v;
          v.push_back(details);
          tree.push(details.name, v);
        }

      } while (Process32NextW(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
  }
}
