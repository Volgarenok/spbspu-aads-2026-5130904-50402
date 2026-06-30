#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP

#include <stdexcept>
#include <utility>

namespace pozdnyakov
{

  namespace detail
  {
    template< typename Key, typename Value >
    struct TreeNode;
  }

  template< class Key, class Value >
  class BSTIterator
  {
  private:
    using Node = detail::TreeNode< Key, Value >;
    Node *current;

  public:
    explicit BSTIterator(Node *node = nullptr):
      current(node)
    {}

    std::pair< const Key &, Value & > operator*() const
    {
      return {current->key, current->value};
    }

    Node *getNode() const
    {
      return current;
    }

    BSTIterator &operator++()
    {
      if (!current) {
        return *this;
      }

      if (current->right) {
        current = current->right;
        while (current->left) {
          current = current->left;
        }
      } else {
        Node *parentNode = current->parent;
        while (parentNode && current == parentNode->right) {
          current = parentNode;
          parentNode = parentNode->parent;
        }
        current = parentNode;
      }
      return *this;
    }

    BSTIterator operator++(int)
    {
      BSTIterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const BSTIterator &other) const
    {
      return current == other.current;
    }

    bool operator!=(const BSTIterator &other) const
    {
      return current != other.current;
    }
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  private:
    using Node = detail::TreeNode< Key, Value >;
    const Node *current;

  public:
    explicit BSTConstIterator(const Node *node = nullptr):
      current(node)
    {}

    BSTConstIterator(const BSTIterator< Key, Value > &other):
      current(other.getNode())
    {}

    std::pair< const Key &, const Value & > operator*() const
    {
      return {current->key, current->value};
    }

    const Node *getNode() const
    {
      return current;
    }

    BSTConstIterator &operator++()
    {
      if (!current) {
        return *this;
      }

      if (current->right) {
        current = current->right;
        while (current->left) {
          current = current->left;
        }
      } else {
        const Node *parentNode = current->parent;
        while (parentNode && current == parentNode->right) {
          current = parentNode;
          parentNode = parentNode->parent;
        }
        current = parentNode;
      }
      return *this;
    }

    BSTConstIterator operator++(int)
    {
      BSTConstIterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const BSTConstIterator &other) const
    {
      return current == other.current;
    }

    bool operator!=(const BSTConstIterator &other) const
    {
      return current != other.current;
    }
  };

}

#endif
