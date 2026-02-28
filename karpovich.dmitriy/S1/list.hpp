#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
namespace karpovich
{
  template< class T >
  class List {
    Node* head_;
  public:
    List();
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List& operator=(const List< T >& other);
    List& operator=(List< T >&& other);
    ~List();

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;

    void push_front(const T&);
    void push_back(const T&);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const;
  };


  template< class T >
  List< T >::List():
    head_(new Node< T >{T(), nullptr, nullptr})
  {}

  template< class T >
  List< T >::List(const List< T >& other):
    head_(nullptr)
  {
    Node* cur = other.head;
    while (cur) {
      push_back(cur->val);
      cur = cur->next;
    }
  }

  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    head_(other.head)
  {
    other.head = nullptr;
  }

  template< class T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    if (this != other) {
      clear();
      Node* cur = other.head;
      while (cur) {
        push_back(cur->val);
      }
    }
    return this;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& other)
  {
    if (this != other) {
      clear();
      head = other.head;
      other.head = nullptr;
    }
    return this;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }
}
#endif
