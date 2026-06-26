#include "commands.hpp"
#include <algorithm>


bool muh::isInt(const std::string& str)
{
  if (str.empty())
  {
    return false;
  }
  for (size_t i = 0; i < str.size(); ++i)
  {
    if (!(str[i] >= '0' && str[i] <= '9') && !(i == 0 && str[i] == '-'))
    {
      return false;
    }
  }
  return true;
}

void muh::cmdPrint(std::istream& in, std::ostream& out, ManyDatasets_t& ds)
{
  std::string name;
  if (!(in >>name))
  {
    throw std::runtime_error("Invalid input");
  }

  if (!ds.has(name))
  {
    throw std::runtime_error("dataset not found");
  }

  const Dataset_t& dset = ds.get(name);
  if (dset.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  out << name;
  for (muh::BSConstIterator<int, std::string> it = dset.cbegin(); it != dset.cend(); ++it)
  {
    out << " " << it->first << " " << it->second;
  }
  out << "\n";
}

void muh::cmdComplement(std::istream& in, std::ostream&, ManyDatasets_t& ds)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Invalid input");
  }

  if (!ds.has(name1) || !ds.has(name2))
  {
    throw std::runtime_error("Incorrect datasets");
  }

  const Dataset_t& ds1 = ds.get(name1);
  const Dataset_t& ds2 = ds.get(name2);
  Dataset_t result;

  for (muh::BSConstIterator< int, std::string > it = ds1.begin(); it != ds1.end(); ++it)
  {
    if (!ds2.has(it->first))
    {
      result.push(it->first, it->second);
    }
  }

  ds.push(newName, result);
}

void muh::cmdIntersect(std::istream& in, std::ostream&, ManyDatasets_t& ds)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Invalid input");
  }

  if (!ds.has(name1) || !ds.has(name2))
  {
    throw std::runtime_error("Incorrect datasets");
  }

  const Dataset_t& ds1 = ds.get(name1);
  const Dataset_t& ds2 = ds.get(name2);
  Dataset_t result;

  for (muh::BSConstIterator< int, std::string > it = ds1.begin(); it != ds1.end(); ++it)
  {
    if (ds2.has(it->first))
    {
      result.push(it->first, it->second);
    }
  }

  ds.push(newName, result);
}

void muh::cmdUnion(std::istream& in, std::ostream&, ManyDatasets_t& ds)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Invalid input");
  }

  if (!ds.has(name1) || !ds.has(name2))
  {
    throw std::runtime_error("Incorrect datasets");
  }

  const Dataset_t& ds1 = ds.get(name1);
  const Dataset_t& ds2 = ds.get(name2);
  Dataset_t result;

  for (muh::BSConstIterator< int, std::string > it = ds1.begin(); it != ds1.end(); ++it)
  {
    result.push(it->first, it->second);
  }

  for (muh::BSConstIterator< int, std::string > it = ds2.begin(); it != ds2.end(); ++it)
  {
    if (!result.has(it->first))
    {
      result.push(it->first, it->second);
    }
  }

  ds.push(newName, result);
}
