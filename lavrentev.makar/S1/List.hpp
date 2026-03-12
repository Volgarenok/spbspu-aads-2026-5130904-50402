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
    //LIter(typename std::pair<std::string, lavrentev::List<int>> *node) : curr(node) {};

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
    //LCIter(typename std::pair<std::string, lavrentev::List<int>> *node) : curr(node) {};

    void printList(std::ostream &os);

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

    List<T>() = default;
    List<T>(const List<T> &);
    List<T>(List<T> &&);
    ~List<T>() noexcept;
    List<T> &operator=(const List<T> &);
    List<T> &operator=(List<T> &&);

    void clear();
    LIter<T> insert(LIter<T> h, const T &v);
    LIter<T> begin();
    LCIter<T> cbegin() const;

  private:
    //T val;
    Node *head;
  };

  lavrentev::List<std::pair<std::string, lavrentev::List<int>>> getline(std::istream &in);

} // namespace lavrentev

inline lavrentev::List<std::pair<std::string, lavrentev::List<int>>> lavrentev::getline(std::istream &in)
{
  lavrentev::List<std::pair<std::string, lavrentev::List<int>>> sequences{};
  std::pair<std::string, lavrentev::List<int>> pair;
  std::string name;
  lavrentev::List<int> seq{};
  auto it = seq.begin();
  auto iterator = sequences.begin();

  if (!(in >> name))
  {
    return sequences;
  }

  int value;
  try
  {
    while (true)
    {
      if (in >> value)
      {
        it = seq.insert(it, value);
        //++it;
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

/*template <class T>
void lavrentev::LCIter<T>::printList(std::ostream &os)
{
  LCIter<T> it = *this;
  while(it.curr){
    it.printPair(os);
    ++it;
  }
  os << "\n";
}*/

template <class T> void lavrentev::LCIter<T>::printList(std::ostream &os)
{
  LCIter<T> it = *this;

  while (it.curr)
  {
    os << (*it).first << " ";

    const lavrentev::List<int> &valList = (*it).second;
    lavrentev::LCIter<int> valIt = valList.cbegin(); // problem?

    while (valIt != lavrentev::LCIter<int>())
    {
      os << *valIt << " ";
      ++valIt;
    }

    os << "\n";
    ++it;
  }
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
  if(this != &other)
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

  if (h.curr == nullptr)
  {
    newNode->next = head;
    head = newNode;
  }
  else
  {
    newNode->next = h.curr->next;
    h.curr->next = newNode;
  }

  return LIter<T>(newNode);
}

template <class T> lavrentev::LIter<T> lavrentev::List<T>::begin()
{
  return LIter<T>(head);
}

template <class T> lavrentev::LCIter<T> lavrentev::List<T>::cbegin() const
{
  return LCIter<T>(head);
}

#endif