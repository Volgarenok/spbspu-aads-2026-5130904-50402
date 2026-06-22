#include "Actions.hpp"
#include <iostream>
#include <limits>
#include <string>

int main()
{
  novikov::MatrixHash db;

  using Command = void (*)(std::istream &, std::ostream &, novikov::MatrixHash &);
  novikov::CuckooHash< std::string, Command > cmds;
  cmds.insert("create", novikov::Create);
  cmds.insert("show", novikov::Show);
  cmds.insert("delete", novikov::Delete);
  cmds.insert("change", novikov::Change);
  cmds.insert("add-row", novikov::AddRow);
  cmds.insert("add-col", novikov::AddCol);
  cmds.insert("swap-rows", novikov::SwapRows);
  cmds.insert("swap-cols", novikov::SwapCols);
  cmds.insert("delete-row", novikov::DeleteRow);
  cmds.insert("delete-col", novikov::DeleteCol);
  cmds.insert("sum", novikov::Sum);
  cmds.insert("sub", novikov::Sub);
  cmds.insert("mul", novikov::Mul);
  cmds.insert("scalar-mul", novikov::ScalarMul);
  cmds.insert("transpose", novikov::Transpose);
  cmds.insert("det", novikov::Det);
  cmds.insert("lu", novikov::Lu);
  cmds.insert("solve", novikov::Solve);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.get(cmd)(std::cin, std::cout, db);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
