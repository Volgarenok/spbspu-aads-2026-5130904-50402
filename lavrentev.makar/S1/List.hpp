#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>

namespace lavrentev
{
  template< class T >
  class List;

  template< class T >
  class LIter
  {
  public:
    bool operator==(const LIter< T >& other) const noexcept;
    bool operator!=(const LIter< T >& other) const noexcept;
    LIter< T >& operator++() noexcept;
    T& operator*();

    LIter():
      curr(nullptr)
    {}
    LIter(typename lavrentev::List< T >::Node* node):
      curr(node)
    {}

    void printList(std::ostream& os);
    void printNames(std::ostream& os);

  private:
    friend class List< T >;
    typename List< T >::Node* curr = nullptr;
  };

  template< class T >
  class LCIter
  {
  public:
    bool operator==(const LCIter< T >& other) const noexcept;
    bool operator!=(const LCIter< T >& other) const noexcept;
    LCIter< T >& operator++() noexcept;
    const T& operator*() const;

    LCIter():
      curr(nullptr) {};
    explicit LCIter(typename List< T >::Node* node):
      curr(node) {};

  private:
    friend class List< T >;
    typename List< T >::Node* curr = nullptr;
  };

  template< class T >
  class List
  {
  public:
    struct Node
    {
      T val;
      Node* next;
    };

    List() noexcept:
      head(nullptr) {};
    List(const List&);
    List(List&&) noexcept;
    ~List() noexcept;
    List< T >& operator=(const List< T >&);
    List< T >& operator=(List< T >&&) noexcept;

    void clear();
    LIter< T > insert(LIter< T > h, const T& v);
    LCIter< T > insert(LCIter< T > h, const T& v);
    LIter< T > popFront();
    LIter< T > begin();
    LCIter< T > cbegin() const;
    LIter< T > end();
    LCIter< T > cend() const;
    T* front();
    const T* front() const;
    bool empty() const;
    void pushFront(const T& v);

  private:
    Node* head;
  };

  lavrentev::List< std::pair< std::string, lavrentev::List< size_t > > > getline(std::istream& in);
  void printTrans(lavrentev::List< std::pair< std::string, lavrentev::List< size_t > > > arr);
} // namespace lavrentev

inline lavrentev::List< std::pair< std::string, lavrentev::List< size_t > > > lavrentev::getline(std::istream& in)
{
  lavrentev::List< std::pair< std::string, lavrentev::List< size_t > > > sequences{};
  std::string name;
  auto iterator = sequences.begin();

  while (in >> name)
  {
    lavrentev::List< size_t > seq{};
    auto it = seq.begin();
    size_t value;

    while (in >> value)
    {
      it = seq.insert(it, value);
    }

    iterator = sequences.insert(iterator, {name, seq});
    if (in.eof())
    {
      break;
    }
    in.clear();
  }

  return sequences;
}

inline void lavrentev::printTrans(lavrentev::List< std::pair< std::string, lavrentev::List< size_t > > > arr)
{
  lavrentev::LIter< std::pair< std::string, lavrentev::List< size_t > > > iterator = arr.begin();
  lavrentev::List< lavrentev::LIter< size_t > > iters = {};
  while (iterator != arr.end())
  {
    lavrentev::LIter< size_t > it = {(*iterator).second.begin()};
    iters.insert(iters.end(), it);
    ++iterator;
  }
  lavrentev::LIter< lavrentev::LIter< size_t > > itersIt = iters.begin();
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

template< class T >
bool lavrentev::LIter< T >::operator==(const LIter< T >& other) const noexcept
{
  return curr == other.curr;
}

template< class T >
bool lavrentev::LIter< T >::operator!=(const LIter< T >& other) const noexcept
{
  return curr != other.curr;
}

template< class T >
lavrentev::LIter< T >& lavrentev::LIter< T >::operator++() noexcept
{
  if (curr != nullptr)
  {
    curr = curr->next;
  }
  return *this;
}

template< class T >
T& lavrentev::LIter< T >::operator*()
{
  return curr->val;
}

template< class T >
bool lavrentev::LCIter< T >::operator==(const LCIter< T >& other) const noexcept
{
  return curr == other.curr;
}

template< class T >
bool lavrentev::LCIter< T >::operator!=(const LCIter< T >& other) const noexcept
{
  return curr != other.curr;
}

template< class T >
lavrentev::LCIter< T >& lavrentev::LCIter< T >::operator++() noexcept
{
  if (curr != nullptr)
  {
    curr = curr->next;
  }
  return *this;
}

template< class T >
const T& lavrentev::LCIter< T >::operator*() const
{
  return curr->val;
}

template< class T >
void lavrentev::LIter< T >::printList(std::ostream& os)
{
  LIter< T > it = *this;
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

template< class T >
void lavrentev::LIter< T >::printNames(std::ostream& os)
{
  LIter< T > it = *this;
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

template< class T >
lavrentev::List< T >::List(const List< T >& other):
  head(nullptr)
{
  if (other.head == nullptr)
  {
    return;
  }
  try
  {
    head = new Node{ other.head->val, nullptr };
    Node* curr = head;
    Node* otherCurr = other.head->next;

    while (otherCurr != nullptr)
    {
      curr->next = new Node{ otherCurr->val, nullptr };
      curr = curr->next;
      otherCurr = otherCurr->next;
    }
  }
  catch (...)
  {
    clear();
    throw;
  }
}

template< class T >
lavrentev::List< T >::List(List< T >&& other) noexcept:
  head(other.head)
{
  other.head = nullptr;
}

template< class T >
lavrentev::List< T >::~List() noexcept
{
  clear();
}

template< class T >
lavrentev::List< T >& lavrentev::List< T >::operator=(const List< T >& other)
{
  if (this != &other)
  {
    List< T > temp(other);
    std::swap(this->head, temp);
  }
  return *this;
}

template< class T >
lavrentev::List< T >& lavrentev::List< T >::operator=(lavrentev::List< T >&& other) noexcept
{
  if (this != &other)
  {
    clear();
    head = other.head;
    other.head = nullptr;
  }

  return *this;
}

template< class T >
void lavrentev::List< T >::clear()
{
  while (head)
  {
    Node* n = head->next;
    delete head;
    head = n;
  }
}

template< class T >
lavrentev::LIter< T > lavrentev::List< T >::insert(lavrentev::LIter< T > h, const T& v)
{
  Node* newNode = new Node;
  newNode->val = v;
  newNode->next = nullptr;

  if (head == nullptr)
  {
    head = newNode;
  }
  else if (h.curr == nullptr)
  {
    Node* current = head;
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

  return LIter< T >(newNode);
}

template< class T >
lavrentev::LCIter< T > lavrentev::List< T >::insert(lavrentev::LCIter< T > h, const T& v)
{
  Node* newNode = new Node;
  newNode->val = v;
  newNode->next = nullptr;

  if (head == nullptr)
  {
    head = newNode;
  }
  else if (h.curr == nullptr)
  {
    Node* current = head;
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

  return LCIter< T >(newNode);
}

template< class T >
lavrentev::LIter< T > lavrentev::List< T >::popFront()
{
  if (head == nullptr)
  {
    return LIter< T >{};
  }
  LIter< T > it = head;
  ++it;
  delete head;
  head = it.curr;
  return it;
}

template< class T >
lavrentev::LIter< T > lavrentev::List< T >::begin()
{
  return LIter< T >(head);
}

template< class T >
lavrentev::LCIter< T > lavrentev::List< T >::cbegin() const
{
  return LCIter< T >(head);
}

template< class T >
lavrentev::LIter< T > lavrentev::List< T >::end()
{
  return LIter< T >(nullptr);
}

template< class T >
lavrentev::LCIter< T > lavrentev::List< T >::cend() const
{
  return LCIter< T >(nullptr);
}

template< class T >
T* lavrentev::List< T >::front()
{
  if (head == nullptr)
  {
    return nullptr;
  }
  lavrentev::LIter< T > h = this->begin();
  return h.curr->val;
}

template< class T >
const T* lavrentev::List< T >::front() const
{
  if (head == nullptr)
  {
    nullptr;
  }
  lavrentev::LCIter< T > h = this->cbegin();
  return h.curr->val;
}

template< class T >
bool lavrentev::List< T >::empty() const
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

template< class T >
void lavrentev::List< T >::pushFront(const T& value)
{
  Node* newNode = new Node{value, head};
  head = newNode;
}

#endif
