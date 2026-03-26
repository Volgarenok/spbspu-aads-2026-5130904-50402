#include <iostream>
#include <utility>
#include <string>
#include <limits>

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
  LIter(): node(nullptr) {}
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
  LCIter(): node(nullptr) {}
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
    try {
      LIter< T > tail = before_begin();
      for (LCIter< T > it = other.cbegin(); it != other.cend(); ++it) {
        tail = insert_after(tail, *it);
      }
    } catch (...) {
      clear();
      delete head;
      throw;
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

  const T& front() const {
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

  std::string name;
  while (in >> name) {

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

int main() {
  auto data = readInput< int >(std::cin);

  if (data.empty()) {
    std::cout << "0\n";
    return 0;
  }

  // вывод имён
  bool first = true;
  for (auto it = data.cbegin(); it != data.cend(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << it->first;
    first = false;
  }
  std::cout << "\n";

  // итераторы текущих позиций в каждом подсписке
  List< LCIter< int > > positions;
  {
    LIter< LCIter< int > > pos_tail = positions.before_begin();
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
      pos_tail = positions.insert_after(pos_tail, it->second.cbegin());
    }
  }

  // суммы зип-последовательностей
  List< int > sums;
  LIter< int > sums_tail = sums.before_begin();

  LCIter< int > end_marker;
  bool any_remaining = true;
  while (any_remaining) {
    any_remaining = false;
    bool line_started = false;
    int sum = 0;

    for (auto pit = positions.begin(); pit != positions.end(); ++pit) {
      if (*pit != end_marker) {
        any_remaining = true;
        int val = **pit;
        if (line_started) {
          std::cout << " ";
        }
        std::cout << val;
        line_started = true;

        if (val > 0 && sum > std::numeric_limits< int >::max() - val) {
          std::cerr << "overflow" << "\n";
          return 1;
        }

        sum += val;
        ++(*pit);
      }
    }

    if (any_remaining) {
      std::cout << "\n";
      sums_tail = sums.insert_after(sums_tail, sum);
    }
  }

  // вывод сумм
  if (!sums.empty()) {
    first = true;
    for (auto it = sums.cbegin(); it != sums.cend(); ++it) {
      if (!first) {
        std::cout << " ";
      }
      std::cout << *it;
      first = false;
    }
    std::cout << "\n";
  }

  return 0;
}
