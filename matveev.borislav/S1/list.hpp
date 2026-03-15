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

private:
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

private:
  const void* node_;
};

template< class T >
class List
{
public:
  List()
    : head_(nullptr)
  {
  }

  LIter< T > begin()
  {
    LIter< T > it;
    it.node_ = head_;
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
    it.node_ = head_;
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

private:
  struct Node
  {
    Node(const T& value)
      : data(value),
        next(nullptr)
    {
    }

    T data;
    Node* next;
  };

  Node* head_;
};

}

#endif
