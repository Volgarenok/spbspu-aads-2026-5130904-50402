#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>

namespace lavrentev
{
  template <class T> class List;

  template <class T> class LIter
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

  template <class T> class LCIter
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

  template <class T> class List
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
    lavrentev::LIter<T> remove(const T &val);

  private:
    Node *head;
  };

  lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>>
  getline(std::istream &in);
  void printTrans(lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>> arr);
} // namespace lavrentev

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

template <class T> lavrentev::LIter<T> &lavrentev::LIter<T>::operator++()
{
  if (curr != nullptr)
  {
    curr = curr->next;
  }
  return *this;
}

template <class T> T &lavrentev::LIter<T>::operator*()
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

template <class T> lavrentev::LCIter<T> &lavrentev::LCIter<T>::operator++()
{
  if (curr != nullptr)
  {
    curr = curr->next;
  }
  return *this;
}

template <class T> const T &lavrentev::LCIter<T>::operator*() const
{
  return curr->val;
}

template <class T> void lavrentev::LIter<T>::printList(std::ostream &os)
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

template <class T> void lavrentev::LIter<T>::printNames(std::ostream &os)
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
lavrentev::List<T>::List(const List<T> &other) : head(nullptr)
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

template <class T> lavrentev::List<T>::List(List<T> &&other) : head(other.head)
{
  other.head = nullptr;
}

template <class T> lavrentev::List<T>::~List() noexcept
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

template <class T> void lavrentev::List<T>::clear()
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

template <class T> lavrentev::LIter<T> lavrentev::List<T>::popFront()
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

template <class T> lavrentev::LIter<T> lavrentev::List<T>::begin()
{
  return LIter<T>(head);
}

template <class T> lavrentev::LCIter<T> lavrentev::List<T>::cbegin() const
{
  return LCIter<T>(head);
}

template <class T> lavrentev::LIter<T> lavrentev::List<T>::end()
{
  return LIter<T>(nullptr);
}

template <class T> lavrentev::LCIter<T> lavrentev::List<T>::cend() const
{
  return LCIter<T>(nullptr);
}

template <class T> T &lavrentev::List<T>::front()
{
  if (head == nullptr)
  {
    throw std::out_of_range("Empty list");
  }
  lavrentev::LIter<T> h = this->begin();
  return h.curr->val;
}

template <class T> const T &lavrentev::List<T>::front() const
{
  if (head == nullptr)
  {
    throw std::out_of_range("Empty list");
  }
  lavrentev::LCIter<T> h = this->cbegin();
  return h.curr->val;
}

template <class T> bool lavrentev::List<T>::empty() const
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

template <class T> void lavrentev::List<T>::pushFront(const T &value)
{
  Node *newNode = new Node{value, head};
  head = newNode;
}

template <class T> lavrentev::LIter<T> lavrentev::List<T>::remove(const T &val)
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

#endif
