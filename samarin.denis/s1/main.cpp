#include <iostream>
#include <stdio.h>
#include <utility>

int main() {

}

template< class T >
struct Node {
  T value;
  Node< T > * next;
};

template< class T > class List;

template< class T >
class LIter {
  friend class List< T >;
  Node< T > * node;
  explicit LIter(Node< T > * n): node(n) {}
public:
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
    LIter< T > tmp = *this;
    node = node->next;
    return tmp;
  }
};

template< class T >
class List {
  Node< T > * head;
public:
  List():
    head(new Node< T >)
  {
    head->next = nullptr;
  }

  ~List() {
    clear();
    delete head;
  }

  void push_front(const T& v) {
    Node< T > * node = new Node< T >;
    node->value = v;
    node->next = head->next;
    head->next = node;
  }

  LIter< T > insert_after(LIter< T > pos, const T& v) {
    Node< T > * node = new Node< T >;
    node->value = v;
    node->next = pos.node->next;
    pos.node->next = node;
    return LIter< T >(node);
  }

  LIter< T > before_begin() { return LIter< T >(head); }

  void clear();
  LIter< T > begin() { return LIter< T >(head->next); }
  LIter< T > end()   { return LIter< T >(nullptr); }
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
  Node * temp = head->next;

  while (temp != nullptr) {
    Node * temp2 = temp->next;
    delete temp;
    temp = temp2;
  }
  head->next = nullptr;
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