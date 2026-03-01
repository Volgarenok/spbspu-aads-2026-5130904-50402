#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iterators.hpp"
namespace karpovich
{
  template < class T > class List
  {
    Node< T > *fake_;
    size_t size_;

  public:
    List();
    List(const List< T > &other);
    List(List< T > &&other) noexcept;
    List &operator=(const List< T > &other);
    List &operator=(List< T > &&other) noexcept;
    ~List();

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > begin() const;
    LCIter< T > end() const;

    void push_front(const T &);
    void push_back(const T &);
    void pop_front();
    void pop_back();
    void clear();
    size_t size() const;
  };

  template < class T >
  List< T >::List():
    fake_(new Node< T >{T(), nullptr, nullptr}),
    size_(0)
  {
    fake_->next = fake_;
    fake_->prev = fake_;
  }

  template < class T >
  List< T >::List(const List< T > &other):
    size_(0),
    fake_(new Node< T >{T(), nullptr, nullptr})
  {
    fake_->next = fake_;
    fake_->prev = fake_;
    Node< T > *cur = other.fake_->next;
    while (cur != other.fake_) {
      push_back(cur->val);
      cur = cur->next;
    }
  }

  template < class T >
  List< T >::List(List< T > &&other) noexcept:
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = new Node< T >{T(), nullptr, nullptr};
    other.fake_->next = other.fake_;
    other.fake_->prev = other.fake_;
    other.size_ = 0;
  }

  template < class T > List< T > &List< T >::operator=(const List< T > &other)
  {
    if (this != &other) {
      clear();
      Node< T > *cur = other.fake_->next;
      while (cur != other.fake_) {
        push_back(cur->val);
        cur = cur->next;
      }
    }
    return *this;
  }

  template < class T > List< T > &List< T >::operator=(List< T > &&other) noexcept
  {
    if (this != &other) {
      clear();
      delete fake_;
      fake_ = other.fake_;
      size_ = other.size_;
      other.fake_ = new Node< T >{T(), nullptr, nullptr};
      other.fake_->next = other.fake_;
      other.fake_->prev = other.fake_;
      other.size_ = 0;
    }
    return *this;
  }

  template < class T > List< T >::~List()
  {
    clear();
    delete fake_;
  }
}
#endif
