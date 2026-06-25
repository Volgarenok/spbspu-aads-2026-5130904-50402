#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TreeImpl.hpp"
#include <limits>

using tree_t = vishnevskiy::BSTree<int, std::string, bool (*)(int, int)>;

bool intComp(int a, int b)
{
  return a < b;
}

bool stringComp(std::string a, std::string b)
{
  return a < b;
}

using dsTree_t = vishnevskiy::BSTree<std::string, tree_t*, bool (*)(std::string, std::string)>;

void load(const std::string& filename, dsTree_t& tree)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }
  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
    {
      continue;
    }
    std::istringstream iss(line);
    std::string datasetName;
    iss >> datasetName;
    tree_t* dsTree = new tree_t(intComp);
    int key;
    std::string value;
    while (iss >> key >> value)
    {
      dsTree->push(key, value);
    }
    tree.push(datasetName, dsTree);
  }
}

void print(std::ostream& o, std::istream& in, dsTree_t& tree)
{
  std::string datasetName;
  in >> datasetName;
  if (!tree.has(datasetName))
  {
    throw std::logic_error("Cannot find name!");
  }
  tree_t* dsTree = tree.get(datasetName);
  vishnevskiy::BSIterator<int, std::string> it = dsTree->begin();
  if (it.isEnd())
  {
    o << "<EMPTY>\n";
    return;
  }
  o << datasetName;
  while (!it.isEnd())
  {
    o << " " << *it.getKey() << " " << *it.getVal();
    it.next();
  }
  o << "\n";
}

void complement(std::ostream&, std::istream& in, dsTree_t& tree)
{
  std::string newDataset, dataset1, dataset2;
  in >> newDataset >> dataset1 >> dataset2;
  if (!tree.has(dataset1) || !tree.has(dataset2))
  {
    throw std::logic_error("Cannot find name!");
  }
  tree_t* ds1 = tree.get(dataset1);
  tree_t* ds2 = tree.get(dataset2);
  tree_t* result = new tree_t(intComp);
  vishnevskiy::BSIterator<int, std::string> it = ds1->begin();
  while (!it.isEnd())
  {
    int key = *it.getKey();
    if (!ds2->has(key))
    {
      result->push(key, *it.getVal());
    }
    it.next();
  }
  if (tree.has(newDataset))
  {
    delete tree.get(newDataset);
  }
  tree.push(newDataset, result);
}

void intersect(std::ostream&, std::istream& in, dsTree_t& tree)
{
  std::string newDataset, dataset1, dataset2;
  in >> newDataset >> dataset1 >> dataset2;
  if (!tree.has(dataset1) || !tree.has(dataset2))
  {
    throw std::logic_error("Cannot find name!");
  }
  tree_t* ds1 = tree.get(dataset1);
  tree_t* ds2 = tree.get(dataset2);
  tree_t* result = new tree_t(intComp);
  vishnevskiy::BSIterator<int, std::string> it = ds1->begin();
  while (!it.isEnd())
  {
    int key = *it.getKey();
    if (ds2->has(key))
    {
      result -> push(key, *it.getVal());
    }
    it.next();
  }
  if (tree.has(newDataset))
  {
    delete tree.get(newDataset);
  }
  tree.push(newDataset, result);
}

void unionCmd(std::ostream&, std::istream& in, dsTree_t& tree)
{
  std::string newDataset, dataset1, dataset2;
  in >> newDataset >> dataset1 >> dataset2;
  if (!tree.has(dataset1) || !tree.has(dataset2))
  {
    throw std::logic_error("Cannot find name!");
  }
  tree_t* ds1 = tree.get(dataset1);
  tree_t* ds2 = tree.get(dataset2);
  tree_t* result = new tree_t(intComp);

  vishnevskiy::BSIterator<int, std::string> it = ds1->begin();
  while (!it.isEnd())
  {
    result -> push(*it.getKey(), *it.getVal());
    it.next();
  }
  it = ds2 -> begin();
  while (!it.isEnd())
  {
    int key = *it.getKey();
    if (!result -> has(key))
    {
      result -> push(key, *it.getVal());
    }
    it.next();
  }
  if (tree.has(newDataset))
  {
    delete tree.get(newDataset);
  }
  tree.push(newDataset, result);
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    return 1;
  }
  std::string filename = argv[1];
  using cm_t = void(*)(std::ostream&, std::istream&, vishnevskiy::BSTree<std::string, tree_t*, bool (*)(std::string, std::string)>&);
  using cmtree_t = vishnevskiy::BSTree<std::string, cm_t, bool (*)(std::string, std::string)>;
  dsTree_t tree(stringComp);
  try
  {
    load(filename, tree);
  }
  catch (const std::runtime_error&)
  {
    std::cerr << "Bad file!\n";
    return 1;
  }
  cmtree_t cmds(stringComp);
  cmds.push("print", print);
  cmds.push("complement", complement);
  cmds.push("intersect", intersect);
  cmds.push("union", unionCmd);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.get(cmd)(std::cout, std::cin, tree);
    }
    catch (const std::runtime_error& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input!\n";
    return 1;
  }

  vishnevskiy::BSIterator<std::string, tree_t*> it = tree.begin();
  while (!it.isEnd())
  {
    delete *(it.getVal());
    it.next();
  }
  return 0;
}
