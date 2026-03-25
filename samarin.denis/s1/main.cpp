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

  List(const List< T >& other):
    head(new Node< T >)
  {
    head->next = nullptr;
    LIter< T > tail = before_begin();
    for (LIter< T > it = other.begin(); it != other.end(); ++it) {
      tail = insert_after(tail, *it);
    }
  }

  List(List< T >&& other) noexcept:
    head(other.head)
  {
    other.head = new Node< T >;
    other.head->next = nullptr;
  }

  List< T >& operator=(const List< T >& other) {
    if (this != &other) {
      List< T > tmp(other);
      std::swap(head, tmp.head);
    }
    return *this;
  }

  List< T >& operator=(List< T >&& other) noexcept {
    if (this != &other) {
      std::swap(head, other.head);
    }
    return *this;
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

  T& front() {
    return head->next->value;
  }

  void pop_front() {
    Node< T > * temp = head->next;
    head->next = temp->next;
    delete temp;
  }

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