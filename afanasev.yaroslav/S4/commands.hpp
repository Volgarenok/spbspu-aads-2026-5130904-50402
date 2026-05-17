#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "BiTree/BiTree.hpp"

namespace afanasev
{
  using Dataset = BSTree< int, std::string >;
  using Datasets = BSTree< std::string, Dataset >;

  void cmdPrint(std::istream & in, std::ostream & out, Datasets & ds);
  void cmdComplement(std::istream & in, std::ostream & out, Datasets & ds);
  void cmdIntersect(std::istream & in, std::ostream & out, Datasets & ds);
  void cmdUnion(std::istream & in, std::ostream & out, Datasets & ds);
}

void afanasev::cmdPrint(std::istream & in, std::ostream & out, Datasets & ds)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("Invalid input");
  }

  try
  {
    const Dataset & dat = ds.get(name);
    if (!dat.size())
    {
      out << "<EMPTY>\n";
      return;
    }
    out << name;
    for (BSTConstIterator< int, std::string > it = dat.begin(); it != dat.end(); ++it)
    {
      std::pair< const int, std::string > pair = *it;
      out << ' ' << pair.first << ' ' << pair.second;
    }
    out << '\n';
  }
  catch (const std::out_of_range &)
  {
    throw std::runtime_error("Dataset not found");
  }
}

void afanasev::cmdComplement(std::istream & in, std::ostream & out, Datasets & ds)
{

}

void afanasev::cmdIntersect(std::istream & in, std::ostream & out, Datasets & ds)
{

}

void afanasev::cmdUnion(std::istream & in, std::ostream & out, Datasets & ds)
{

}

#endif
