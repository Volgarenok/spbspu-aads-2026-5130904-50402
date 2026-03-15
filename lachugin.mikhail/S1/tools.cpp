#include "tools.hpp"
#include "list.hpp"

lachugin::List < lachugin::pair > lachugin::getline(std::istream& in)
{
  List < pair > listOfLists{};
  Node < pair >* currOfLists = nullptr;

  std::string name;
  try
  {
    while (in >> name)
    {
      auto* list = new List< int >{};
      Node< int >* curr = nullptr;
      int value;

      while(in >> value)
      {
        if(curr == nullptr)
        {
          curr = list->add(value);
        }
        else
        {
          curr = list->addNext(value, curr);
        }
      }

      in.clear();
      pair p{name, list};

      if (currOfLists == nullptr)
      {
        currOfLists = listOfLists.add(p);
      }
      else
      {
        currOfLists = listOfLists.addNext(p, currOfLists);
      }
    }
  }
  catch (const std::bad_alloc& e)
  {
    std::cout << e.what();
    listOfLists.clear();
    throw;
  }

  return listOfLists;
}