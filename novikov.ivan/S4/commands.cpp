#include <bilist.hpp>
#include "commands.hpp"
#include "BSTree.hpp"

void novikov::print(std::istream& in, std::ostream& out, BSTList& bstl)
{
  std::string name;
  in >> name;
  LIter< BSTree< size_t, std::string, std::less< size_t > > > it = bstl.begin();
  while (it != bstl.end())
  {
    if ((*it).getName() == name)
    {
      break;
    }
    ++it;
  }
  if (it == bstl.end())
  {
    throw std::out_of_range("Tree does not exist");
  }
  BSTIterator< size_t, std::string > bstIt = (*it).begin();
  if (bstIt == (*it).end())
  {
    out << "<EMPTY>" << '\n';
    return;
  }
  out << name << " " << (*bstIt).first << " " << (*bstIt).second;
  ++bstIt;
  for (; bstIt != (*it).end(); ++bstIt)
  {
    out << " " << (*bstIt).first << " " << (*bstIt).second;
  }
  out << '\n';
}

void novikov::complement(std::istream& in, std::ostream&, BSTList& bstl)
{
  std::string newBstName, BstName1, BstName2;
  in >> newBstName >> BstName1 >> BstName2;
  BSTree< size_t, std::string, std::less< size_t > > newBst;
  newBst.setName(newBstName);
  LIter< BSTree< size_t, std::string, std::less< size_t > > > it = bstl.begin();
  BSTree< size_t, std::string, std::less< size_t > >* bst1 = nullptr;
  BSTree< size_t, std::string, std::less< size_t > >* bst2 = nullptr;
  bool is1 = false, is2 = false;
  while (it != bstl.end())
  {
    if ((*it).getName() == BstName1)
    {
      is1 = true;
      bst1 = &(*it);
    }
    if ((*it).getName() == BstName2)
    {
      is2 = true;
      bst2 = &(*it);
    }
    if (is1 && is2)
    {
      break;
    }
    ++it;
  }
  if (!is1 || !is2)
  {
    throw std::out_of_range("");
  }
  BSTIterator< size_t, std::string > bstIt = (*bst1).begin();
  if (bstIt == (*bst1).end())
  {
    bstl.pushFront(std::move(newBst));
    return;
  }
  for (; bstIt != (*bst1).end(); ++bstIt)
  {
    if (!(*bst2).has((*bstIt).first))
    {
      newBst[(*bstIt).first] = (*bstIt).second;
    }
  }
  bstl.pushFront(std::move(newBst));
}

void novikov::intersect(std::istream& in, std::ostream&, BSTList& bstl)
{
  std::string newBstName, BstName1, BstName2;
  in >> newBstName >> BstName1 >> BstName2;
  BSTree< size_t, std::string, std::less< size_t > > newBst;
  newBst.setName(newBstName);
  LIter< BSTree< size_t, std::string, std::less< size_t > > > it = bstl.begin();
  BSTree< size_t, std::string, std::less< size_t > >* bst1 = nullptr;
  BSTree< size_t, std::string, std::less< size_t > >* bst2 = nullptr;
  bool is1 = false, is2 = false;
  while (it != bstl.end())
  {
    if ((*it).getName() == BstName1)
    {
      is1 = true;
      bst1 = &(*it);
    }
    if ((*it).getName() == BstName2)
    {
      is2 = true;
      bst2 = &(*it);
    }
    if (is1 && is2)
    {
      break;
    }
    ++it;
  }
  if (!is1 || !is2)
  {
    throw std::out_of_range("");
  }
  BSTIterator< size_t, std::string > bstIt = (*bst1).begin();
  if (bstIt == (*bst1).end())
  {
    bstl.pushFront(std::move(newBst));
    return;
  }
  for (; bstIt != (*bst1).end(); ++bstIt)
  {
    if ((*bst2).has((*bstIt).first))
    {
      newBst[(*bstIt).first] = (*bstIt).second;
    }
  }
  bstl.pushFront(std::move(newBst));
}

void novikov::union_(std::istream& in, std::ostream&, BSTList& bstl)
{
  std::string newBstName, BstName1, BstName2;
  in >> newBstName >> BstName1 >> BstName2;
  BSTree< size_t, std::string, std::less< size_t > > newBst;
  LIter< BSTree< size_t, std::string, std::less< size_t > > > it = bstl.begin();
  BSTree< size_t, std::string, std::less< size_t > >* bst1 = nullptr;
  BSTree< size_t, std::string, std::less< size_t > >* bst2 = nullptr;
  bool is1 = false, is2 = false;
  while (it != bstl.end())
  {
    if ((*it).getName() == BstName1)
    {
      is1 = true;
      bst1 = &(*it);
    }
    if ((*it).getName() == BstName2)
    {
      is2 = true;
      bst2 = &(*it);
    }
    if (is1 && is2)
    {
      break;
    }
    ++it;
  }
  if (!is1 || !is2)
  {
    throw std::out_of_range("");
  }
  newBst = *bst1;
  newBst.setName(newBstName);
  BSTIterator< size_t, std::string > bstIt = (*bst2).begin();
  for (; bstIt != (*bst2).end(); ++bstIt)
  {
    if (!newBst.has((*bstIt).first))
    {
      newBst[(*bstIt).first] = (*bstIt).second;
    }
  }
  bstl.pushFront(std::move(newBst));
}
