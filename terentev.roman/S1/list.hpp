#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>

namespace terentev
{
  template< typename T >
  class List
  {
  private:
    struct Node
    {
      T data;
      Node *next;

      Node(const T &value, Node *nextNode = nullptr):
        data(value),
        next(nextNode)
      {}
    };

  public:
    class LIter
    {
      friend class List< T >;

    public:
      LIter():
        node_(nullptr)
      {}

      explicit LIter(Node *node):
        node_(node)
      {}

      T &operator*() const
      {
        return node_->data;
      }

      T *operator->() const
      {
        return &(node_->data);
      }

      LIter &operator++()
      {
        if (node_ != nullptr) {
          node_ = node_->next;
        }
        return *this;
      }

      LIter operator++(int)
      {
        LIter temp(*this);
        ++(*this);
        return temp;
      }

      bool operator==(const LIter &other) const
      {
        return node_ == other.node_;
      }

      bool operator!=(const LIter &other) const
      {
        return node_ != other.node_;
      }

    private:
      Node *node_;
    };

    class LCIter
    {
      friend class List< T >;

    public:
      LCIter():
        node_(nullptr)
      {}

      explicit LCIter(const Node *node):
        node_(node)
      {}

      LCIter(const LIter &other):
        node_(other.node_)
      {}

      const T &operator*() const
      {
        return node_->data;
      }

      const T *operator->() const
      {
        return &(node_->data);
      }

      LCIter &operator++()
      {
        if (node_ != nullptr) {
          node_ = node_->next;
        }
        return *this;
      }

      LCIter operator++(int)
      {
        LCIter temp(*this);
        ++(*this);
        return temp;
      }

      bool operator==(const LCIter &other) const
      {
        return node_ == other.node_;
      }

      bool operator!=(const LCIter &other) const
      {
        return node_ != other.node_;
      }

    private:
      const Node *node_;
    };

    List():
      head_(nullptr),
      size_(0)
    {}

    List(const List &other):
      head_(nullptr),
      size_(0)
    {
      if (other.head_ == nullptr) {
        return;
      }

      head_ = new Node(other.head_->data);
      size_ = 1;

      Node *current = head_;
      Node *source = other.head_->next;

      try {
        while (source != nullptr) {
          current->next = new Node(source->data);
          current = current->next;
          source = source->next;
          ++size_;
        }
      } catch (...) {
        clear();
        throw;
      }
    }

    List(List &&other) noexcept:
      head_(other.head_),
      size_(other.size_)
    {
      other.head_ = nullptr;
      other.size_ = 0;
    }

    ~List()
    {
      clear();
    }

    List &operator=(const List &other)
    {
      if (this != &other) {
        List temp(other);
        swap(temp);
      }
      return *this;
    }

    List &operator=(List &&other) noexcept
    {
      if (this != &other) {
        clear();
        head_ = other.head_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    void pushFront(const T &value)
    {
      head_ = new Node(value, head_);
      ++size_;
    }

    void insertAfter(LIter pos, const T &value)
    {
      if (pos.node_ == nullptr) {
        return;
      }

      Node *newNode = new Node(value, pos.node_->next);
      pos.node_->next = newNode;
      ++size_;
    }

    void popFront()
    {
      if (head_ == nullptr) {
        return;
      }

      Node *temp = head_;
      head_ = head_->next;
      delete temp;
      --size_;
    }

    void clear()
    {
      while (head_ != nullptr) {
        popFront();
      }
    }

    void swap(List &other) noexcept
    {
      std::swap(head_, other.head_);
      std::swap(size_, other.size_);
    }

    bool isEmpty() const
    {
      return size_ == 0;
    }

    size_t getSize() const
    {
      return size_;
    }

    T &front()
    {
      return head_->data;
    }

    const T &front() const
    {
      return head_->data;
    }

    LIter begin()
    {
      return LIter(head_);
    }

    LIter end()
    {
      return LIter(nullptr);
    }

    LCIter begin() const
    {
      return LCIter(head_);
    }

    LCIter end() const
    {
      return LCIter(nullptr);
    }

    LCIter cbegin() const
    {
      return LCIter(head_);
    }

    LCIter cend() const
    {
      return LCIter(nullptr);
    }

  private:
    Node *head_;
    size_t size_;
  };
}

#endif
