#include <stdexcept>
#include <cctype>
#include <fstream>
#include <iomanip>
#include "commands.hpp"
#include "ProcessInfo.hpp"
#include "ProcessUtilities.hpp"

namespace novikov
{
  void snap(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string snap_name;
    if (!(in >> snap_name)) return;

    if (snaps.has(snap_name))
    {
      out << "<SNAPSHOT NAME EXISTS>\n";
      throw std::runtime_error("Snapshot name exists");
    }

    Snapshot* new_snap = new Snapshot();
    new_snap->name = snap_name;
    new_snap->parent = nullptr;

    takeSystemSnapshot(new_snap->processTree);

    snaps.push(snap_name, new_snap);

    out << "Snapshot " << snap_name << " has been created.\n";
  }

  void displayAllProcesses(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string snap_name;
    if (!(in >> snap_name)) return;

    if (!snaps.has(snap_name))
    {
      out << "<SNAPSHOT NOT FOUND>\n";
      throw std::runtime_error("Snapshot not found");
    }

    Snapshot* snap = snaps.get(snap_name);
    ProcessSnapTree& tree = snap->processTree;

    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
      out << toUtf8((*it).first) << " (PIDs: ";

      const auto& procs = (*it).second;
      for (size_t i = 0; i < procs.size(); ++i)
      {
        out << procs[i].pid;
        if (i + 1 < procs.size())
        {
          out << ", ";
        }
      }
      out << ")\n";
    }
  }

  void getProcessDetails(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string arg;
    std::string snap_name;
    if (!(in >> arg >> snap_name)) return;

    if (!snaps.has(snap_name))
    {
      out << "<SNAPSHOT NOT FOUND>\n";
      throw std::runtime_error("Snapshot not found");
    }

    Snapshot* snap = snaps.get(snap_name);
    ProcessSnapTree& tree = snap->processTree;

    bool is_pid = !arg.empty();
    for (char c : arg)
    {
      if (!std::isdigit(static_cast< unsigned char >(c)))
      {
        is_pid = false;
        break;
      }
    }

    if (is_pid)
    {
      DWORD target_pid = std::stoul(arg);
      bool found = false;

      for (auto it = tree.begin(); it != tree.end(); ++it)
      {
        for (const auto& proc : (*it).second)
        {
          if (proc.pid == target_pid)
          {
            out << "Process " << target_pid << " details:\n";
            out << "Name: " << toUtf8(proc.name) << "\n";
            if (proc.accessDenied)
            {
              out << "Access denied.\n";
              out << "Path: N/A\n";
              out << "CPU usage: N/A\n";
              out << "RAM usage: N/A\n";
            }
            else
            {
              out << "Path: " << toUtf8(proc.path) << "\n";
              out << "CPU usage: " << std::fixed << std::setprecision(1) << proc.cpuUsage << " %\n";
              out << "RAM usage: " << proc.ramUsage << " MB\n";
            }
            found = true;
            break;
          }
        }
        if (found) break;
      }

      if (!found)
      {
        out << "<PROCESS NOT FOUND>\n";
        throw std::runtime_error("Process not found");
      }
    }
    else
    {
      std::wstring target_name = toWString(arg);

      if (!tree.has(target_name))
      {
        out << "<PROCESS NOT FOUND>\n";
        throw std::runtime_error("Process not found");
      }

      const auto& procs = tree.get(target_name);

      double total_cpu = 0.0;
      size_t total_ram = 0;
      size_t denied_count = 0;

      out << arg << " details:\n";
      out << "PIDs: ";
      for (size_t i = 0; i < procs.size(); ++i)
      {
        out << procs[i].pid;
        if (i + 1 < procs.size()) out << ", ";
        if (procs[i].accessDenied)
        {
          ++denied_count;
        }
        else
        {
          total_cpu += procs[i].cpuUsage;
          total_ram += procs[i].ramUsage;
        }
      }
      out << "\n";
      if (denied_count == procs.size())
      {
        out << "Access denied.\n";
        out << "CPU usage: N/A\n";
        out << "RAM usage: N/A\n";
      }
      else
      {
        out << "CPU usage: " << std::fixed << std::setprecision(1) << total_cpu << " %\n";
        out << "RAM usage: " << total_ram << " MB\n";

        if (denied_count > 0)
        {
          out << "Note: " << denied_count << " process(es) excluded from metrics (access denied)\n";
        }
      }
    }
  }

  void compareSnapshots(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string snap_name1, snap_name2;
    if (!(in >> snap_name1 >> snap_name2)) return;

    if (!snaps.has(snap_name1) || !snaps.has(snap_name2))
    {
      out << "<SNAPSHOT NOT FOUND>\n";
      throw std::runtime_error("One or both snapshots not found");
    }

    ProcessSnapTree& tree1 = snaps.get(snap_name1)->processTree;
    ProcessSnapTree& tree2 = snaps.get(snap_name2)->processTree;

    out << "New processes in " << snap_name2 << " compared to " << snap_name1 << ":\n";

    for (auto it = tree2.begin(); it != tree2.end(); ++it)
    {
      const std::wstring& proc_name = (*it).first;

      if (!tree1.has(proc_name))
      {
        out << toUtf8(proc_name) << "\n";
      }
    }
  }

  void filterRAM(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string src_name;
    size_t min_ram;
    if (!(in >> src_name >> min_ram)) return;

    if (!snaps.has(src_name))
    {
      out << "<SNAPSHOT NOT FOUND>\n";
      throw std::runtime_error("Source snapshot not found");
    }

    std::string new_name = src_name + "_filteredRAM_" + std::to_string(min_ram);

    if (snaps.has(new_name))
    {
      out << "<SNAPSHOT NAME EXISTS>\n";
      throw std::runtime_error("Destination snapshot name already exists");
    }

    Snapshot* parent_snap = snaps.get(src_name);
    ProcessSnapTree& src_tree = parent_snap->processTree;
    ProcessSnapTree filtered_tree;

    out << "Processes which RAM usage exceeds " << min_ram << " MB:\n";

    for (auto it = src_tree.begin(); it != src_tree.end(); ++it)
    {
      std::vector<ProcessDetails> matched_procs;

      for (const auto& proc : (*it).second)
      {
        if (proc.ramUsage > min_ram)
        {
          matched_procs.push_back(proc);
          out << proc.pid << " " << toUtf8(proc.name) << "\n";
        }
      }

      if (!matched_procs.empty())
      {
        filtered_tree.push((*it).first, matched_procs);
      }
    }

    Snapshot* child_snap = new Snapshot();
    child_snap->name = new_name;
    child_snap->processTree = filtered_tree;
    child_snap->parent = parent_snap;

    parent_snap->children.push_back(child_snap);
    snaps.push(new_name, child_snap);

    out << "Created filtered snapshot: " << new_name << "\n";
  }

  void filterCPU(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string src_name;
    double min_percentage;
    if (!(in >> src_name >> min_percentage)) return;

    if (!snaps.has(src_name))
    {
      out << "<SNAPSHOT NOT FOUND>\n";
      throw std::runtime_error("Source snapshot not found");
    }

    std::string cpu_str = std::to_string(min_percentage);
    cpu_str.erase(cpu_str.find_last_not_of('0') + 1, std::string::npos);
    if (cpu_str.back() == '.') cpu_str.pop_back();

    std::string new_name = src_name + "_filteredCPU_" + cpu_str;

    if (snaps.has(new_name))
    {
      out << "<SNAPSHOT NAME EXISTS>\n";
      throw std::runtime_error("Destination snapshot name already exists");
    }

    Snapshot* parent_snap = snaps.get(src_name);
    ProcessSnapTree& src_tree = parent_snap->processTree;
    ProcessSnapTree filtered_tree;

    out << "Processes which CPU usage exceeds " << min_percentage << " %:\n";

    for (auto it = src_tree.begin(); it != src_tree.end(); ++it)
    {
      std::vector<ProcessDetails> matched_procs;

      for (const auto& proc : (*it).second)
      {
        if (proc.cpuUsage > min_percentage)
        {
          matched_procs.push_back(proc);
          out << proc.pid << " " << toUtf8(proc.name) << "\n";
        }
      }

      if (!matched_procs.empty())
      {
        filtered_tree.push((*it).first, matched_procs);
      }
    }

    Snapshot* child_snap = new Snapshot();
    child_snap->name = new_name;
    child_snap->processTree = filtered_tree;
    child_snap->parent = parent_snap;

    parent_snap->children.push_back(child_snap);
    snaps.push(new_name, child_snap);

    out << "Created filtered snapshot: " << new_name << "\n";
  }

  void exportSnapshot(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string snap_name, file_name;
    if (!(in >> snap_name >> file_name)) return;

    if (!snaps.has(snap_name))
    {
      out << "<SNAPSHOT NOT FOUND>\n";
      throw std::runtime_error("Snapshot not found");
    }

    std::string full_path = file_name + ".txt";

    if (fileExists(full_path))
    {
      out << "<FILE ALREADY EXISTS>\n";
      throw std::runtime_error("File already exists");
    }

    std::ofstream outFile(full_path);
    if (!outFile.is_open())
    {
      out << "<ACCESS DENIED>\n";
      throw std::runtime_error("Access denied");
    }

    Snapshot* snap = snaps.get(snap_name);
    ProcessSnapTree& tree = snap->processTree;

    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
      std::string proc_name_utf8 = toUtf8((*it).first);
      const auto& procs = (*it).second;

      for (const auto& proc : procs)
      {
        outFile << proc.pid << " " << proc_name_utf8 << "\n";
      }
    }

    outFile.close();
    out << "Snapshot " << snap_name << " has been saved to " << full_path << ".\n";
  }

  void deleteSnapshot(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    std::string snap_name;
    if (!(in >> snap_name)) return;

    if (!snaps.has(snap_name))
    {
      out << "<SNAPSHOT NOT FOUND>\n";
      throw std::runtime_error("Snapshot not found");
    }

    out << "WARNING: deletion of this snapshot will delete all its filters.\n";
    out << "Are you sure you want to delete snapshot '" << snap_name
        << "' and all its inherited filters? (yes/no): ";

    std::string confirmation;
    if (!(in >> confirmation)) return;

    for (char &c : confirmation)
    {
      c = std::tolower(static_cast< unsigned char >(c));
    }

    if (confirmation != "yes" && confirmation != "y")
    {
      out << "Deletion cancelled.\n";
      return;
    }

    Snapshot* snap_to_delete = snaps.get(snap_name);
    Snapshot* parent_node = snap_to_delete->parent;

    if (parent_node != nullptr)
    {
      auto& siblings = parent_node->children;
      siblings.erase(std::remove(siblings.begin(), siblings.end(), snap_to_delete), siblings.end());
    }

    freeSnapshotSubtree(snap_to_delete, snaps);

    out << "Snapshot " << snap_name << " and all its filters have been deleted.\n";
  }

  void list(std::istream&, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    if (snaps.height() == 0)
    {
      out << "No snapshots available.\n";
      return;
    }

    for (auto it = snaps.begin(); it != snaps.end(); ++it)
    {
      out << (*it).first;
      auto next_it = it;
      ++next_it;
      if (next_it != snaps.end())
      {
        out << ", ";
      }
    }
    out << ".\n";
  }

  void clearAll(std::istream& in, std::ostream& out, novikov::SnapshotTree& snaps)
  {
    out << "WARNING: running this command will delete all available snapshots.\n";
    out << "Are you sure you want to clear ALL snapshots? (yes/no): ";

    std::string confirmation;
    if (!(in >> confirmation)) return;

    for (char &c : confirmation)
    {
      c = std::tolower(static_cast< unsigned char >(c));
    }

    if (confirmation != "yes" && confirmation != "y")
    {
      out << "Deletion cancelled.\n";
      return;
    }

    while (snaps.height() > 0)
    {
      auto it = snaps.begin();
      Snapshot* snap = (*it).second;

      if (snap->parent == nullptr)
      {
        freeSnapshotSubtree(snap, snaps);
      }
      else
      {
        snaps.drop((*it).first);
      }
    }

    out << "All snapshots have been cleared.\n";
  }

  void help(std::istream&, std::ostream& out, novikov::SnapshotTree&)
  {
    out << "Available commands:\n"
        << "  snap <snap_name>                         - Take a snapshot of all system processes\n"
        << "  displayAllProcesses <snap_name>          - Display processes and their PIDs\n"
        << "  getProcessDetails <name/pid> <snap>      - Display full resources usage for a process\n"
        << "  compareSnapshots <snap1> <snap2>         - Show new processes in snap2 compared to snap1\n"
        << "  filterRAM <snap_name> <min_ram>          - Create a filtered snapshot by RAM usage\n"
        << "  filterCPU <snap_name> <min_percentage>   - Create a filtered snapshot by CPU usage\n"
        << "  exportSnapshot <snap_name> <file>        - Export snapshot data to <file>.txt\n"
        << "  delete <snap_name>                       - Delete snapshot and all its inherited filters\n"
        << "  list                                     - List all active snapshots\n"
        << "  clearAll                                 - Delete all snapshots\n"
        << "  help                                     - Show this help reference\n";
  }
}
