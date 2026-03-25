#include <iostream>
#include <utility>
#include <string>

int main() {

}

template< class T >
struct Node {
  T value;
  Node< T > * next;
};

template< class T > class List;
template< class T > class LCIter;

template< class T >
class LIter {
  friend class List< T >;
  friend class LCIter< T >;
  Node< T > * node;
  explicit LIter(Node< T > * n): node(n) {}
public:
  T& operator*() {
    return node->value;
  }
  T* operator->() {
    return &node->value;
  }
  LIter< T >& operator++() {
    node = node->next;
    return *this;
  }
  LIter< T > operator++(int) {
    LIter< T > tmp = *this;
    node = node->next;
    return tmp;
  }
  bool operator==(const LIter< T >& other) const { return node == other.node; }
  bool operator!=(const LIter< T >& other) const { return node != other.node; }
};

template< class T >
class LCIter {
  friend class List< T >;
  const Node< T > * node;
  explicit LCIter(const Node< T > * n): node(n) {}
public:
  LCIter(const LIter< T >& it): node(it.node) {}
  const T& operator*() const {
    return node->value;
  }
  const T* operator->() const {
    return &node->value;
  }
  LCIter< T >& operator++() {
    node = node->next;
    return *this;
  }
  LCIter< T > operator++(int) {
    LCIter< T > tmp = *this;
    node = node->next;
    return tmp;
  }
  bool operator==(const LCIter< T >& other) const { return node == other.node; }
  bool operator!=(const LCIter< T >& other) const { return node != other.node; }
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
    for (LCIter< T > it = other.cbegin(); it != other.cend(); ++it) {
      tail = insert_after(tail, *it);
    }
  }

  List(List< T >&& other) noexcept:
    head(other.head)
  {
    other.head = nullptr;
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
      clear();
      delete head;
      head = other.head;
      other.head = nullptr;
    }
    return *this;
  }

  ~List() {
    if (head != nullptr) {
      clear();
      delete head;
    }
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

  bool empty() const { return head->next == nullptr; }

  void clear();
  LIter< T >  begin()         { return LIter< T >(head->next); }
  LIter< T >  end()           { return LIter< T >(nullptr); }
  LCIter< T > begin()  const  { return LCIter< T >(head->next); }
  LCIter< T > end()    const  { return LCIter< T >(nullptr); }
  LCIter< T > cbegin() const  { return LCIter< T >(head->next); }
  LCIter< T > cend()   const  { return LCIter< T >(nullptr); }
  LCIter< T > cbefore_begin() const { return LCIter< T >(head); }
};

template< class T >
List< std::pair< std::string, List< T > > > readInput(std::istream& in) {
  List< std::pair< std::string, List< T > > > res;
  LIter< std::pair< std::string, List< T > > > res_tail = res.before_begin();

  while (!in.eof() && !in.bad()) {
    std::string name;
    in >> name;
    if (in.fail()) {
      break;
    }

    List< T > sublist;
    LIter< T > tail = sublist.before_begin();
    T val;
    while (in >> val) {
      tail = sublist.insert_after(tail, val);
    }

    if (!in.eof()) {
      in.clear();
    }

    res_tail = res.insert_after(res_tail, std::make_pair(name, std::move(sublist)));
  }

  return res;
}

template<class T>
void List< T >::clear() {
  Node< T > * temp = head->next;

  while (temp != nullptr) {
    Node< T > * temp2 = temp->next;
    delete temp;
    temp = temp2;
  }
  head->next = nullptr;
}
