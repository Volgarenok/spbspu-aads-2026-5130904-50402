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
  LIter<T> begin() { return LIter< T >(head->next); }
};

