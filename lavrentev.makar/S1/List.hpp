#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>

namespace lavrentev
{
  template <class T> class List;

  template <class T> class LIter
  {
  public:
    LIter() : curr(nullptr) {}
    explicit LIter(List<T> *n) : curr(n) {};
    void clear();

  private:
    friend class List<T>;
    List<T> *curr;
  };

  template <class T> class LCIter
  {
    public:
      LCIter() : curr(nullptr) {}
      explicit LCIter(const List<T> *n) : curr(n) {}
      void printList();
    private:
      friend class List<T>;
      const List<T> *curr;
  };

  template <class T> class List
  {
  public:
    List() : val{}, next(nullptr) {}
    List(const T &v, List<T> *n = nullptr) : val(v), next(n) {}

    ~List() = default;

    T retVal() const;
    List<T> *retNext() const;
    void addVal(int k);
    void addNext(List<T> *aft);
    
  private:
    T val;
    List<T> *next;
  };

  std::pair<std::string, List<int>*>* getline(std::istream &in, size_t &size);
  //void input(std::istream &in, LIter<int> *iter, char *s);

  template <class T> List<T> *add(List<T> *h, const T &v);
  template <class T> List<T> *insert(List<T> *h, const T &v);

  void expand(std::pair<std::string, List<int>*> **arr, size_t &cap,
              size_t size);

  template <class T> void clear(LIter<T> *k);
} // namespace lavrentev

#endif