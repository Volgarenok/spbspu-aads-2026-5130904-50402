#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <functional>

namespace lavrentev
{
  template <class T>
  class List;

  template <class T>
  class LIter
  {
  public:
    bool operator==(const LIter<T> &other) const;
    bool operator!=(const LIter<T> &other) const;
    LIter<T> &operator++();
    T &operator*();

    LIter() : curr(nullptr) {};
    LIter(typename lavrentev::List<T>::Node *node) : curr(node) {};

    void printList(std::ostream &os);
    void printNames(std::ostream &os);

  private:
    friend class List<T>;
    typename List<T>::Node *curr = nullptr;
  };

  template <class T>
  class LCIter
  {
  public:
    bool operator==(const LCIter<T> &other) const;
    bool operator!=(const LCIter<T> &other) const;
    LCIter<T> &operator++();
    const T &operator*() const;

    LCIter() : curr(nullptr) {};
    explicit LCIter(typename List<T>::Node *node) : curr(node) {};

  private:
    friend class List<T>;
    typename List<T>::Node *curr = nullptr;
  };

  template <class T>
  class List
  {
  public:
    struct Node
    {
      T val;
      Node *next;
    };

    List() : head(nullptr) {};
    List(const List &);
    List(List &&);
    ~List() noexcept;
    List<T> &operator=(const List<T> &);
    List<T> &operator=(List<T> &&);

    void clear();
    LIter<T> insert(LIter<T> h, const T &v);
    LCIter<T> insert(LCIter<T> h, const T &v);
    LIter<T> popFront();
    LIter<T> begin();
    LCIter<T> cbegin() const;
    LIter<T> end();
    LCIter<T> cend() const;
    T &front();
    const T &front() const;
    bool empty() const;
    void pushFront(const T &v);
    LIter<T> remove(const T &val);
    LIter<T> erase(LIter<T> val);

    void splice(LIter<T> pos, List<T> &other);
    void splice(LIter<T> pos, List<T> &other, LIter<T> it);
    void splice(LIter<T> pos, List<T> &other, LIter<T> first, LIter<T> last);

    void merge(List<T> &other);
    template <class Comp> void merge(List<T> &other, Comp comp);

    void sort();

    template <class Comp>
    void sort(Comp comp);

    template <class Pred>
    void partition(Pred p);

  private:
    Node *head;

    Node *get_tail(Node *node) const;
    Node *get_prev(Node *node) const;
    void transfer(LIter<T> pos, List<T> &other, Node *first, Node *last);
  };

  lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>>
  getline(std::istream &in);
  void printTrans(lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>> arr);
}

inline lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>>
lavrentev::getline(std::istream &in)
{
  lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>> sequences{};
  std::pair<std::string, lavrentev::List<size_t>> pair;
  std::string name;
  lavrentev::List<size_t> seq{};
  auto it = seq.begin();
  auto iterator = sequences.begin();

  if (!(in >> name))
  {
    return sequences;
  }

  size_t value;
  try
  {
    while (true)
    {
      if (in >> value)
      {
        it = seq.insert(it, value);
      }
      else
      {
        if (in.eof())
        {
          pair.first = name;
          pair.second = seq;
          iterator = sequences.insert(iterator, pair);
          break;
        }

        in.clear();
        std::string bad;
        if (in >> bad)
        {
          pair.first = name;
          pair.second = seq;
          iterator = sequences.insert(iterator, pair);
          name = bad;
          seq.clear();
          it = seq.begin();
        }
      }
    }
  } catch (...)
  {
    seq.clear();
    sequences.clear();
    throw;
  }
  return sequences;
}

inline void lavrentev::printTrans(lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>> arr)
{
  lavrentev::LIter<std::pair<std::string, lavrentev::List<size_t>>> iterator = arr.begin();
  lavrentev::List<lavrentev::LIter<size_t>> iters = {};
  while (iterator != arr.end())
  {
    lavrentev::LIter<size_t> it = {(*iterator).second.begin()};
    iters.insert(iters.end(), it);
    ++iterator;
  }
  lavrentev::LIter<lavrentev::LIter<size_t>> itersIt = iters.begin();
  while (true)
  {
    size_t sum = 0;
    iterator = arr.begin();
    bool first = true;
    while (itersIt != iters.end())
    {
      if (*itersIt != (*iterator).second.end())
      {
        size_t k = **itersIt;
        sum = k;
        if (!first)
        {
          std::cout << " ";
        }
        std::cout << k;
        first = false;
        ++(*itersIt);
      }
      ++itersIt;
      ++iterator;
    }
    if (sum == 0)
    {
      break;
    }
    std::cout << "\n";
    itersIt = iters.begin();
  }
}

template <class T>
bool lavrentev::LIter<T>::operator==(const LIter<T> &other) const
{
  return curr == other.curr;
}

template <class T>
bool lavrentev::LIter<T>::operator!=(const LIter<T> &other) const
{
  return curr != other.curr;
}

template <class T>
lavrentev::LIter<T> &lavrentev::LIter<T>::operator++()
{
  if (curr != nullptr)
  {
    curr = curr->next;
  }
  return *this;
}

template <class T>
T &lavrentev::LIter<T>::operator*()
{
  return curr->val;
}

template <class T>
bool lavrentev::LCIter<T>::operator==(const LCIter<T> &other) const
{
  return curr == other.curr;
}

template <class T>
bool lavrentev::LCIter<T>::operator!=(const LCIter<T> &other) const
{
  return curr != other.curr;
}

template <class T>
lavrentev::LCIter<T> &lavrentev::LCIter<T>::operator++()
{
  if (curr != nullptr)
  {
    curr = curr->next;
  }
  return *this;
}

template <class T>
const T &lavrentev::LCIter<T>::operator*() const
{
  return curr->val;
}

template <class T>
void lavrentev::LIter<T>::printList(std::ostream &os)
{
  LIter<T> it = *this;
  bool first = true;
  while (it.curr)
  {
    if (!first)
    {
      std::cout << " ";
    }
    os << *it;
    ++it;
    first = false;
  }
  os << "\n";
}

template <class T>
void lavrentev::LIter<T>::printNames(std::ostream &os)
{
  LIter<T> it = *this;
  bool first = true;
  while (it.curr)
  {
    if (!first)
    {
      os << " ";
    }
    os << (*it).first;
    ++it;
    first = false;
  }
  os << "\n";
}

template <class T>
lavrentev::List<T>::List(const List<T> &other):
  head(nullptr)
{
  if (other.head == nullptr)
  {
    return;
  }

  head = new Node;
  head->val = other.head->val;
  head->next = nullptr;

  Node *curr = head;
  Node *bufCurr = other.head->next;

  while (bufCurr != nullptr)
  {
    curr->next = new Node;
    curr->next->val = bufCurr->val;
    curr->next->next = nullptr;

    curr = curr->next;
    bufCurr = bufCurr->next;
  }
}

template <class T>
lavrentev::List<T>::List(List<T> &&other):
  head(other.head)
{
  other.head = nullptr;
}

template <class T>
lavrentev::List<T>::~List() noexcept
{
  clear();
}

template <class T>
lavrentev::List<T> &lavrentev::List<T>::operator=(const List<T> &other)
{
  if (this != &other)
  {
    clear();

    if (other.head == nullptr)
    {
      return *this;
    }

    head = new Node;
    head->val = other.head->val;
    head->next = nullptr;

    Node *curr = head;
    Node *bufCurr = other.head->next;

    while (bufCurr != nullptr)
    {
      curr->next = new Node;
      curr->next->val = bufCurr->val;
      curr->next->next = nullptr;

      curr = curr->next;
      bufCurr = bufCurr->next;
    }
  }

  return *this;
}

template <class T>
lavrentev::List<T> &lavrentev::List<T>::operator=(lavrentev::List<T> &&other)
{
  if (this != &other)
  {
    clear();
    head = other.head;
    other.head = nullptr;
  }

  return *this;
}

template <class T>
void lavrentev::List<T>::clear()
{
  while (head)
  {
    Node *n = head->next;
    delete head;
    head = n;
  }
}

template <class T>
lavrentev::LIter<T> lavrentev::List<T>::insert(lavrentev::LIter<T> h, const T &v)
{
  Node *newNode = new Node;
  newNode->val = v;
  newNode->next = nullptr;

  if (head == nullptr)
  {
    head = newNode;
  }
  else if (h.curr == nullptr)
  {
    Node *current = head;
    while (current->next != nullptr)
    {
      current = current->next;
    }
    current->next = newNode;
  }
  else
  {
    newNode->next = h.curr->next;
    h.curr->next = newNode;
  }

  return LIter<T>(newNode);
}

template <class T>
lavrentev::LCIter<T> lavrentev::List<T>::insert(lavrentev::LCIter<T> h, const T &v)
{
  Node *newNode = new Node;
  newNode->val = v;
  newNode->next = nullptr;

  if (head == nullptr)
  {
    head = newNode;
  }
  else if (h.curr == nullptr)
  {
    Node *current = head;
    while (current->next != nullptr)
    {
      current = current->next;
    }
    current->next = newNode;
  }
  else
  {
    newNode->next = h.curr->next;
    h.curr->next = newNode;
  }

  return LCIter<T>(newNode);
}

template <class T>
lavrentev::LIter<T> lavrentev::List<T>::popFront()
{
  if (head == nullptr)
  {
    return LIter<T>{};
  }
  LIter<T> it = head;
  ++it;
  delete head;
  head = it.curr;
  return it;
}

template <class T>
lavrentev::LIter<T> lavrentev::List<T>::begin()
{
  return LIter<T>(head);
}

template <class T>
lavrentev::LCIter<T> lavrentev::List<T>::cbegin() const
{
  return LCIter<T>(head);
}

template <class T>
lavrentev::LIter<T> lavrentev::List<T>::end()
{
  return LIter<T>(nullptr);
}

template <class T>
lavrentev::LCIter<T> lavrentev::List<T>::cend() const
{
  return LCIter<T>(nullptr);
}

template <class T>
T &lavrentev::List<T>::front()
{
  if (head == nullptr)
  {
    throw std::out_of_range("Empty list");
  }
  lavrentev::LIter<T> h = this->begin();
  return h.curr->val;
}

template <class T>
const T &lavrentev::List<T>::front() const
{
  if (head == nullptr)
  {
    throw std::out_of_range("Empty list");
  }
  lavrentev::LCIter<T> h = this->cbegin();
  return h.curr->val;
}

template <class T>
bool lavrentev::List<T>::empty() const
{
  if (head == nullptr)
  {
    return true;
  }
  else
  {
    return false;
  }
}

template <class T>
void lavrentev::List<T>::pushFront(const T &value)
{
  Node *newNode = new Node{value, head};
  head = newNode;
}

template <class T>
lavrentev::LIter<T> lavrentev::List<T>::remove(const T &val)
{
  if (head == nullptr) {
    return LIter<T>{};
  }
  if (head->val == val) {
    Node *toDel = head;
    head = head->next;
    delete toDel;
    return begin();
  }
  Node *prev = head;
  while (prev->next != nullptr) {
    if (prev->next->val == val) {
      Node* toDel = prev->next;
      prev->next = toDel->next;
      LIter<T> nextIter(prev->next);
      delete toDel;
      return nextIter;
    }
    prev = prev->next;
  }
  return LIter<T>{};
}

template <class T>
lavrentev::LIter<T> lavrentev::List<T>::erase(lavrentev::LIter<T> val)
{
  if (val == LIter<T>{})
  {
    return val;
  }
  if (head == nullptr)
  {
    return LIter<T>{};
  }
  if (head == val.curr) {
    Node *toDel = head;
    head = head->next;
    delete toDel;
    return begin();
  }
  Node *prev = head;
  while (prev->next != nullptr) {
    if (prev->next == val.curr) {
      Node* toDel = prev->next;
      prev->next = toDel->next;
      LIter<T> nextIter(prev->next);
      delete toDel;
      return nextIter;
    }
    prev = prev->next;
  }
  return LIter<T>{};
}

template <class T>
typename lavrentev::List<T>::Node *lavrentev::List<T>::get_tail(Node *node) const
{
  if (!node)
  {
    return nullptr;
  }
  while (node->next)
  {
    node = node->next;
  }
  return node;
}

template <class T>
typename lavrentev::List<T>::Node *lavrentev::List<T>::get_prev(Node *node) const
{
  if (!head || head == node)
  {
    return nullptr;
  }
  Node *curr = head;
  while (curr->next && curr->next != node)
  {
    curr = curr->next;
  }
  return curr;
}

template <class T>
void lavrentev::List<T>::transfer(LIter<T> pos, List<T> &other, Node *first, Node *last)
{
  if (!first || this == &other)
  {
    return;
  }

  Node *prevOther = other.get_prev(first);
  if (prevOther)
  {
    prevOther->next = last->next;
  }
  else
  {
    other.head = last->next;
  }
  last->next = nullptr;

  if (pos.curr == head)
  {
    last->next = head;
    head = first;
  }
  else if (pos.curr == nullptr)
  {
    if (!head)
    {
      head = first;
    }
    else
    {
      get_tail(head)->next = first;
    }
  }
  else
  {
    Node *prev = get_prev(pos.curr);
    if (prev)
    {
      prev->next = first;
      last->next = pos.curr;
    }
  }
}

template <class T>
void lavrentev::List<T>::splice(LIter<T> pos, List<T> &other)
{
  if (other.head == nullptr || this == &other)
  {
    return;
  }
  transfer(pos, other, other.head, get_tail(other.head));
}

template <class T>
void lavrentev::List<T>::splice(LIter<T> pos, List<T> &other, LIter<T> it)
{
  if (it.curr == nullptr || this == &other)
  {
    return;
  }
  transfer(pos, other, it.curr, it.curr);
}

template <class T>
void lavrentev::List<T>::splice(LIter<T> pos, List<T> &other, LIter<T> first, LIter<T> last)
{
  if (first == last || this == &other)
  {
    return;
  }
  Node *last_node = first.curr;
  while (last_node && last_node->next != last.curr)
  {
    last_node = last_node->next;
  }
  transfer(pos, other, first.curr, last_node);
}

template <class T>
template <class Comp>
void lavrentev::List<T>::merge(List<T> &other, Comp comp)
{
  if (this == &other)
  {
    return;
  }
  Node dum;
  dum.next = nullptr;
  Node *tail = &dum;
  Node *curr1 = head;
  Node *curr2 = other.head;

  while (curr1 && curr2)
  {
    if (comp(curr2->val, curr1->val))
    {
      tail->next = curr2;
      curr2 = curr2->next;
    }
    else
    {
      tail->next = curr1;
      curr1 = curr1->next;
    }
    tail = tail->next;
  }
  tail->next = curr1 ? curr1 : curr2;
  head = dum.next;
  other.head = nullptr;
}

template <class T>
void lavrentev::List<T>::merge(List<T> &other)
{
  merge(other, std::less<T>());
}

template <class T>
template <class Comp>
void lavrentev::List<T>::sort(Comp comp)
{
  if (!head || !head->next)
  {
    return;
  }
  List<T> car;
  List<T> counter[50];
  int fill = 0;
  while (head)
  {
    car.head = head;
    head = head->next;
    car.head->next = nullptr;
    int i = 0;
    while (i < fill && counter[i].head)
    {
      counter[i].merge(car, comp);
      car.head = counter[i].head;
      counter[i].head = nullptr;
      i++;
    }
    counter[i].head = car.head;
    car.head = nullptr;
    if (i == fill)
    {
      fill++;
    }
  }
  for (int i = 1; i < fill; i++)
  {
    counter[i].merge(counter[i - 1], comp);
  }
  head = counter[fill - 1].head;
  counter[fill - 1].head = nullptr;
}

template <class T>
void lavrentev::List<T>::sort()
{
  sort(std::less<T>());
}

template <class T>
template <class Pred>
void lavrentev::List<T>::partition(Pred p)
{
  if (!head || !head->next)
  {
    return;
  }
  Node dT, dF;
  dT.next = nullptr;
  dF.next = nullptr;
  Node *tailT = &dT;
  Node *tailF = &dF;

  Node *curr = head;
  while (curr)
  {
    if (p(curr->val))
    {
      tailT->next = curr;
      tailT = curr;
    }
    else
    {
      tailF->next = curr;
      tailF = curr;
    }
    curr = curr->next;
  }
  tailF->next = nullptr;
  tailT->next = dF.next;
  head = dT.next ? dT.next : dF.next;
}

#endif
