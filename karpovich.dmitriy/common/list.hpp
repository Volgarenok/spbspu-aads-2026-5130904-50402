#ifndef LIST_HPP
#define LIST_HPP
#include <cassert>
#include <cstddef>
#include <memory>
#include "iterators.hpp"
#include "node.hpp"
namespace karpovich
{
  template< class T >
  class List
  {
    details::Node< T > *fake_;
    size_t size_;

  public:
    List();
    List(const List< T > &other);
    List(List< T > &&other) noexcept;
    List &operator=(const List< T > &other);
    List &operator=(List< T > &&other) noexcept;
    ~List() noexcept;

    T &front() noexcept;
    T &back() noexcept;
    const T &front() const noexcept;
    const T &back() const noexcept;
    LIter< T > begin() noexcept;
    LIter< T > end() noexcept;
    LCIter< T > begin() const noexcept;
    LCIter< T > end() const noexcept;
    LCIter< T > cbegin() const noexcept;
    LCIter< T > cend() const noexcept;
    LIter< T > insert(LIter< T > pos, const T &value);
    LIter< T > insert(LIter< T > pos, T &&value);
    LIter< T > erase(LIter< T > pos) noexcept;
    void pushFront(const T &val);
    void pushFront(T &&val);
    void pushBack(const T &val);
    void pushBack(T &&val);
    void popFront() noexcept;
    void popBack() noexcept;
    void clear() noexcept;
    void swap(List< T > &other) noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void splice(LIter< T > position, List< T > &other) noexcept;
    void splice(LIter< T > position, List< T > &other, LIter< T > i) noexcept;
    void splice(LIter< T > position, List< T > &other, LIter< T > first, LIter< T > last) noexcept;
    void sort() noexcept;
    template< class Comparator >
    void sort(Comparator cmp) noexcept;
    void merge(List< T > &other) noexcept;
    template< class Comparator >
    void merge(List< T > &other, Comparator cmp) noexcept;
    template< class Predicate >
    LIter< T > partition(Predicate pred);
  };

  template< class T >
  List< T >::List():
    fake_(new details::Node< T >{T(), nullptr, nullptr}),
    size_(0)
  {
    fake_->next = fake_;
    fake_->prev = fake_;
  }

  template< class T >
  List< T >::List(const List< T > &other):
    fake_(new details::Node< T >{T(), nullptr, nullptr}),
    size_(0)
  {
    fake_->next = fake_;
    fake_->prev = fake_;
    details::Node< T > *cur = other.fake_->next;
    while (cur != other.fake_) {
      pushBack(cur->val);
      cur = cur->next;
    }
  }

  template< class T >
  List< T >::List(List< T > &&other) noexcept:
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = new details::Node< T >{T(), nullptr, nullptr};
    other.fake_->next = other.fake_;
    other.fake_->prev = other.fake_;
    other.size_ = 0;
  }

  template< class T >
  List< T > &List< T >::operator=(const List< T > &other)
  {
    assert(this != std::addressof(other));
    List< T > stub(other);
    swap(stub);
    return *this;
  }

  template< class T >
  List< T > &List< T >::operator=(List< T > &&other) noexcept
  {
    assert(this != std::addressof(other));
    details::Node< T > *newFake = new details::Node< T >{T(), nullptr, nullptr};
    newFake->next = newFake;
    newFake->prev = newFake;
    clear();
    delete fake_;
    fake_ = other.fake_;
    size_ = other.size_;
    other.fake_ = newFake;
    other.size_ = 0;
    return *this;
  }

  template< class T >
  List< T >::~List() noexcept
  {
    clear();
    delete fake_;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    details::Node< T > *cur = fake_->next;
    while (cur != fake_) {
      details::Node< T > *temp = cur->next;
      delete cur;
      cur = temp;
    }
    fake_->next = fake_;
    fake_->prev = fake_;
    size_ = 0;
  }

  template< class T >
  void List< T >::pushBack(const T &val)
  {
    details::Node< T > *node = new details::Node< T >{val, fake_, fake_->prev};
    fake_->prev->next = node;
    fake_->prev = node;
    size_++;
  }

  template< class T >
  void List< T >::pushBack(T &&val)
  {
    details::Node< T > *node = new details::Node< T >{std::move(val), fake_, fake_->prev};
    fake_->prev->next = node;
    fake_->prev = node;
    size_++;
  }

  template< class T >
  void List< T >::pushFront(const T &val)
  {
    details::Node< T > *node = new details::Node< T >{val, fake_->next, fake_};
    fake_->next->prev = node;
    fake_->next = node;
    size_++;
  }

  template< class T >
  void List< T >::pushFront(T &&val)
  {
    details::Node< T > *node = new details::Node< T >{std::move(val), fake_->next, fake_};
    fake_->next->prev = node;
    fake_->next = node;
    size_++;
  }

  template< class T >
  void List< T >::popBack() noexcept
  {
    if (size_ == 0) {
      return;
    }
    details::Node< T > *temp = fake_->prev;
    temp->prev->next = fake_;
    fake_->prev = temp->prev;
    delete temp;
    size_--;
  }

  template< class T >
  void List< T >::popFront() noexcept
  {
    if (size_ == 0) {
      return;
    }
    details::Node< T > *temp = fake_->next;
    temp->next->prev = fake_;
    fake_->next = temp->next;
    delete temp;
    size_--;
  }

  template< class T >
  LIter< T > List< T >::begin() noexcept
  {
    return LIter< T >{fake_->next};
  }

  template< class T >
  LIter< T > List< T >::end() noexcept
  {
    return LIter< T >{fake_};
  }

  template< class T >
  LCIter< T > List< T >::cbegin() const noexcept
  {
    return LCIter< T >{fake_->next};
  }

  template< class T >
  LCIter< T > List< T >::cend() const noexcept
  {
    return LCIter< T >{fake_};
  }

  template< class T >
  void List< T >::swap(List< T > &other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > pos, const T &value)
  {
    details::Node< T > *posNode = pos.ptr_;
    details::Node< T > *newNode = new details::Node< T >{value, posNode, posNode->prev};

    posNode->prev->next = newNode;
    posNode->prev = newNode;
    size_++;

    return LIter< T >{newNode};
  }

  template< class T >
  LIter< T > List< T >::insert(LIter< T > pos, T &&value)
  {
    details::Node< T > *posNode = pos.ptr_;
    details::Node< T > *newNode = new details::Node< T >{std::move(value), posNode, posNode->prev};

    posNode->prev->next = newNode;
    posNode->prev = newNode;
    size_++;

    return LIter< T >{newNode};
  }

  template< class T >
  T &List< T >::front() noexcept
  {
    return fake_->next->val;
  }

  template< class T >
  const T &List< T >::front() const noexcept
  {
    return fake_->next->val;
  }

  template< class T >
  T &List< T >::back() noexcept
  {
    return fake_->prev->val;
  }

  template< class T >
  const T &List< T >::back() const noexcept
  {
    return fake_->prev->val;
  }

  template< class T >
  LIter< T > List< T >::erase(LIter< T > pos) noexcept
  {
    details::Node< T > *node = pos.ptr_;
    details::Node< T > *next = node->next;
    node->prev->next = next;
    next->prev = node->prev;
    delete node;
    size_--;
    return LIter< T >{next};
  }

  template< class T >
  LCIter< T > List< T >::begin() const noexcept
  {
    return LCIter< T >{fake_->next};
  }

  template< class T >
  LCIter< T > List< T >::end() const noexcept
  {
    return LCIter< T >{fake_};
  }

  template< class T >
  void List< T >::splice(LIter< T > position, List< T > &other) noexcept
  {
    if (other.empty()) {
      return;
    }
    details::Node< T > *posNode = position.ptr_;
    details::Node< T > *otherFirst = other.fake_->next;
    details::Node< T > *otherLast = other.fake_->prev;

    otherFirst->prev = posNode->prev;
    posNode->prev->next = otherFirst;
    otherLast->next = posNode;
    posNode->prev = otherLast;

    size_ += other.size_;
    other.fake_->next = other.fake_;
    other.fake_->prev = other.fake_;
    other.size_ = 0;
  }

  template< class T >
  void List< T >::splice(LIter< T > position, List< T > &other, LIter< T > i) noexcept
  {
    if (other.empty() || i == other.end()) {
      return;
    }
    details::Node< T > *node = i.ptr_;

    node->prev->next = node->next;
    node->next->prev = node->prev;
    other.size_--;

    details::Node< T > *posNode = position.ptr_;
    node->next = posNode;
    node->prev = posNode->prev;
    posNode->prev->next = node;
    posNode->prev = node;
    size_++;
  }

  template< class T >
  void List< T >::splice(LIter< T > position, List< T > &other, LIter< T > first, LIter< T > last) noexcept
  {
    if (first == last) {
      return;
    }
    details::Node< T > *fNode = first.ptr_;
    details::Node< T > *lNode = last.ptr_;
    details::Node< T > *rangeLast = lNode->prev;

    fNode->prev->next = lNode;
    lNode->prev = fNode->prev;
    size_t count = 0;
    for (details::Node< T > *cur = fNode; cur != lNode; cur = cur->next) {
      ++count;
    }
    other.size_ -= count;
    size_ += count;
    details::Node< T > *posNode = position.ptr_;
    fNode->prev = posNode->prev;
    posNode->prev->next = fNode;
    rangeLast->next = posNode;
    posNode->prev = rangeLast;
  }
}
#endif
