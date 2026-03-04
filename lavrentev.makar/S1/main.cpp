#include <iostream>

namespace lavrentev
{
  template <class T> class List
  {
    T val;
    List<T> *next;
  };

  template <class T> class LIter
  {
    friend class List<T>;
    List<T> *curr;
  };

  template <class T> class LCIter
  {
    friend class List<T>;
    const List<const T> *curr;
  };

  std::pair<std::string, List<int>> getline(std::istream &in, size_t &n);
}

int main()
{
}
