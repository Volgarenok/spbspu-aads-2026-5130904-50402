#include <iostream>
#include <stdio.h>
#include <utility>

int main() {

}

template< class T > class List;

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

template<class T>
class List {
  struct Node { T value; Node * next; };
  Node* head;
public:
  T value;
  List< T >* next;
  void push(const T& v);
  void clear();
  LIter<T> begin() { return LIter< T >(head->next); }
};

