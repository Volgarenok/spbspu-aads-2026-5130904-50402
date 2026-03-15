#ifndef MATVEEV_LIST_HPP
#define MATVEEV_LIST_HPP

#include <cstddef>

namespace matveev
{

template< class T >
class List;

template< class T >
class LIter
{
  friend class List< T >;

public:
  LIter()
    : node_(nullptr)
  {
  }

  T& operator*() const
  {
    Node* node = reinterpret_cast< Node* >(node_);
    return node->data;
  }

  T* operator->() const
  {
    Node* node = reinterpret_cast< Node* >(node_);
    return &(node->data);
  }

  LIter& operator++()
  {
    Node* node = reinterpret_cast< Node* >(node_);
    node_ = node->next;
    return *this;
  }

  bool operator==(const LIter& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const LIter& other) const
  {
    return node_ != other.node_;
  }

private:
  struct Node
  {
    T data;
    Node* next;
  };

  void* node_;
};

template< class T >
class LCIter
{
  friend class List< T >;

public:
  LCIter()
    : node_(nullptr)
  {
  }

  const T& operator*() const
  {
    const Node* node = reinterpret_cast< const Node* >(node_);
    return node->data;
  }

  const T* operator->() const
  {
    const Node* node = reinterpret_cast< const Node* >(node_);
    return &(node->data);
  }

  LCIter& operator++()
  {
    const Node* node = reinterpret_cast< const Node* >(node_);
    node_ = node->next;
    return *this;
  }

  bool operator==(const LCIter& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const LCIter& other) const
  {
    return node_ != other.node_;
  }

private:
  struct Node
  {
    T data;
    Node* next;
  };

  const void* node_;
};

template< class T >
class List
{
public:
  List()
  {
    sentinel_ = new Node();
  }

  List(const List& other)
  {
    sentinel_ = new Node();
    Node* tail = sentinel_;

    Node* cur = other.sentinel_->next;

    while (cur != nullptr)
    {
      Node* node = new Node(cur->data);
      tail->next = node;
      tail = node;
      cur = cur->next;
    }
  }

  List(List&& other)
  {
    sentinel_ = other.sentinel_;
    other.sentinel_ = new Node();
  }

  List& operator=(const List& other)
  {
    if (this == &other)
    {
      return *this;
    }

    clear();

    Node* tail = sentinel_;
    Node* cur = other.sentinel_->next;

    while (cur != nullptr)
    {
      Node* node = new Node(cur->data);
      tail->next = node;
      tail = node;
      cur = cur->next;
    }

    return *this;
  }

  List& operator=(List&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    clear();
    delete sentinel_;

    sentinel_ = other.sentinel_;
    other.sentinel_ = new Node();

    return *this;
  }

  ~List()
  {
    clear();
    delete sentinel_;
  }

  void removeFront()
  {
    Node* first = sentinel_->next;

    if (first == nullptr)
    {
      return;
    }

    sentinel_->next = first->next;
    delete first;
  }

  void clear()
  {
    while (sentinel_->next != nullptr)
    {
      removeFront();
    }
  }

  LIter< T > begin()
  {
    LIter< T > it;
    it.node_ = sentinel_->next;
    return it;
  }

  LIter< T > end()
  {
    LIter< T > it;
    it.node_ = nullptr;
    return it;
  }

  LCIter< T > begin() const
  {
    LCIter< T > it;
    it.node_ = sentinel_->next;
    return it;
  }

  LCIter< T > end() const
  {
    LCIter< T > it;
    it.node_ = nullptr;
    return it;
  }

  LIter< T > insertAfter(LIter< T > pos, const T& value)
  {
    Node* node = reinterpret_cast< Node* >(pos.node_);

    Node* newNode = new Node(value);
    newNode->next = node->next;
    node->next = newNode;

    LIter< T > it;
    it.node_ = newNode;

    return it;
  }

  void eraseAfter(LIter< T > pos)
  {
    Node* node = reinterpret_cast< Node* >(pos.node_);

    Node* target = node->next;

    if (target == nullptr)
    {
      return;
    }

    node->next = target->next;
    delete target;
  }

private:
  struct Node
  {
    Node()
      : data(),
        next(nullptr)
    {
    }

    Node(const T& value)
      : data(value),
        next(nullptr)
    {
    }

    T data;
    Node* next;
  };

  Node* sentinel_;
};

}

#endif
