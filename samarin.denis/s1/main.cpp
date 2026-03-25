#include <iostream>
#include <stdio.h>
#include <utility>

int main() {

}

template<class T>
class List {
  struct Node { T value; Node * next; };
  Node* head;
public:
  T value;
  List< T >* next;
  void push(const T& v);
  void clear();
  LIter< T > begin() { return LIter< T >(head->next); }
};

template< class T >
class LIter {
  friend class List< T >;
  Node< T > * node;
public:
  explicit LIter(Node< T > * n): node(n) {}
  T& operator*() {
    return node->value;
  }
  T* operator->() {
    return &node->value;
  }
  LIter< T > & operator++() {
    node = node->next;
    return *this;
  }
  LIter< T > operator++(int) {
    Liter< T > tmp = *this;
    node = node->next;
    return tmp;
  }
};

template< class T >
 List< std::pair< std::string, List< T >* > >* readInput(std::istream& in) {

  List< std::pair< std::string, List< T >* > >* res = nullptr;

  while (!in.eof() && !in.bad()) {
    std::string name = "";
    in >> name;
    if (in.fail()) {
      break;
    }

    List< T >* sublist = nullptr;
    T val;
    while (in >> val) {
      List< T >* node = nullptr;
      try {
        node = new List< T >;
      }
      catch (const std::bad_alloc& e) {
        freeList(sublist);
        freeList(res);
        throw;
      }
      node->value = val;
      node->next = nullptr;
      pushBack(sublist, node);
    }

    if (!in.eof()) {
      in.clear();
    }

    std::pair< std::string, List< T >* > p(name, sublist);

    List< std::pair< std::string, List< T >* > >* newNode = nullptr;
    try {
      newNode = new List< std::pair< std::string, List< T >* > >;
    }
    catch (const std::bad_alloc& e) {
      freeList(sublist);
      freeList(res);
      throw;
    }
    newNode->value = p;
    newNode->next = nullptr;
    pushBack(res, newNode);
  }

  return res;
}

template<class T>
void List< T >::clear() {
  List< T > * temp = next;

  while (temp != nullptr) {
    List< T > * temp2 = temp->next;
    delete temp;
    temp = temp2;
  }
  next = nullptr;
}

template< class T >
void clear(List< std::pair< std::string, List< T > * > > * h) {
  List< std::pair< std::string, List< T > * > > * cur = h;
  while (cur != nullptr) {
    List< std::pair< std::string, List< T > * > > * next_outer = cur->next;
    List< T > * inner = cur->value.second;
    while (inner != nullptr) {
      List< T > * next_inner = inner->next;
      delete inner;
      inner = next_inner;
    }
    delete cur;
    cur = next_outer;
  }
}