#ifndef LISTTOOLS_HPP
#define LISTTOOLS_HPP
#include <cstddef>
#include <string>

namespace vishnevskiy
{
  template <class T> class List;
  template <class T> class NamedList;
  template <class T> class LIter;
  template <class T> class LCIter;
  template <class T> class NamedLIter;

  template <class T>
  class List
  {
    public:
      T val;
      List<T>* next;
      List();
      List(T vl, List<T>* nxt);
  };

  template <class T>
  class NamedList
  {
    public:
      std::string name;
      List<T>* data;
      NamedList<T>* next;
      NamedList();
      NamedList(std::string nm, List<T>* dt, NamedList<T>* nxt);
  };

  template <class T>
  class NamedLIter
  {
    public:
      friend class NamedList<T>;
      NamedList<T>* curr;
      NamedLIter();
      NamedLIter(NamedList<T>* h);
      List<T>* value();
      void operator++();
      std::string getName();
      void setCurr(NamedList<T>* h);
      void setData(List<T>* dta);
      bool hasNext();
      void end();
      void insert(List<T>* d, std::string name);
      void clear(NamedLIter* head);
      template <class C>
      void traverse(C func);
  };

  template <class T>
  class LIter
  {
    public:
      friend class List<T>;
      List<T>* curr;
      LIter();
      LIter(List<T>* h);
      T value();
      void operator++();
      void set(List<T>* h);
      bool hasNext();
      void end();
      void insert(T& d);
      void clear(LIter* head);
      template <class C>
      void traverse(C func);
  };

  template <class T>
  class LCIter
  {
    public:
      friend class List<T>;
      const List<T>* curr;
      LCIter();
      LCIter(const List<T>* h);
      void operator++();
      const T* value();
      bool hasNext();
  };
}

#endif
