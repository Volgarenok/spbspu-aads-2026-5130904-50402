#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP

#include <utility>

namespace sogdanov
{

  template <class Key, class Value>
  struct Node
  {
    std::pair<Key, Value> data;
    Node<Key, Value> *left;
    Node<Key, Value> *right;
    Node<Key, Value> *parent;

    Node(const Key &k,
         const Value &v, Node<Key, Value> *p = nullptr);
  };

  template <class Key, class Value>
  Node<Key, Value>::Node(
      const Key &k, const Value &v, Node<Key, Value> *p)
      : data(k, v), left(nullptr), right(nullptr), parent(p)
  {
  }

  template <class Key, class Value>
  class BSTIterator
  {
  public:
    Node<Key, Value> *node;
    Node<Key, Value> *fake_leaf;

    BSTIterator(
        Node<Key, Value> *n, Node<Key, Value> *f);

    std::pair<Key, Value> &operator*() const;

    BSTIterator<Key, Value> &operator++();
    BSTIterator<Key, Value> operator++(int);

    bool operator==(
        const BSTIterator<Key, Value> &other) const;
    bool operator!=(
        const BSTIterator<Key, Value> &other) const;
  };

  template <class Key, class Value>
  class BSTConstIterator
  {
  public:
    const Node<Key, Value> *node;
    const Node<Key, Value> *fake_leaf;

    BSTConstIterator(const Node<Key, Value> *n,
                     const Node<Key, Value> *f);
    BSTConstIterator(const BSTIterator<Key, Value> &it);

    const std::pair<Key, Value> &operator*() const;

    BSTConstIterator<Key, Value> &operator++();
    BSTConstIterator<Key, Value> operator++(int);

    bool operator==(
        const BSTConstIterator<Key, Value> &other) const;
    bool operator!=(
        const BSTConstIterator<Key, Value> &other) const;
  };

  template <class Key, class Value>
  BSTIterator<Key, Value>::BSTIterator(
      Node<Key, Value> *n, Node<Key, Value> *f)
      : node(n), fake_leaf(f)
  {
  }

  template <class Key, class Value>
  std::pair<Key, Value> &BSTIterator<Key, Value>::operator*() const
  {
    return node->data;
  }

  template <class Key, class Value>
  BSTIterator<Key, Value> &BSTIterator<Key, Value>::operator++()
  {
    if (node == fake_leaf)
    {
      return *this;
    }
    if (node->right != fake_leaf)
    {
      node = node->right;
      while (node->left != fake_leaf)
      {
        node = node->left;
      }
    }
    else
    {
      Node<Key, Value> *p = node->parent;
      while (p != nullptr && node == p->right)
      {
        node = p;
        p = p->parent;
      }
      if (p == nullptr)
      {
        node = fake_leaf;
      }
      else
      {
        node = p;
      }
    }
    return *this;
  }

  template <class Key, class Value>
  BSTIterator<Key, Value> BSTIterator<Key, Value>::operator++(int)
  {
    BSTIterator<Key, Value> tmp = *this;
    ++(*this);
    return tmp;
  }

  template <class Key, class Value>
  bool BSTIterator<Key, Value>::operator==(
      const BSTIterator<Key, Value> &other) const
  {
    return node == other.node;
  }

  template <class Key, class Value>
  bool BSTIterator<Key, Value>::operator!=(
      const BSTIterator<Key, Value> &other) const
  {
    return node != other.node;
  }

  template <class Key, class Value>
  BSTConstIterator<Key, Value>::BSTConstIterator(
      const Node<Key, Value> *n, const Node<Key, Value> *f)
      : node(n), fake_leaf(f)
  {
  }

  template <class Key, class Value>
  BSTConstIterator<Key, Value>::BSTConstIterator(
      const BSTIterator<Key, Value> &it)
      : node(it.node), fake_leaf(it.fake_leaf)
  {
  }

  template <class Key, class Value>
  const std::pair<Key, Value> &BSTConstIterator<Key, Value>::operator*() const
  {
    return node->data;
  }

  template <class Key, class Value>
  BSTConstIterator<Key, Value> &BSTConstIterator<Key, Value>::operator++()
  {
    if (node == fake_leaf)
    {
      return *this;
    }
    if (node->right != fake_leaf)
    {
      node = node->right;
      while (node->left != fake_leaf)
      {
        node = node->left;
      }
    }
    else
    {
      const Node<Key, Value> *p = node->parent;
      while (p != nullptr && node == p->right)
      {
        node = p;
        p = p->parent;
      }
      if (p == nullptr)
      {
        node = fake_leaf;
      }
      else
      {
        node = p;
      }
    }
    return *this;
  }

  template <class Key, class Value>
  BSTConstIterator<Key, Value> BSTConstIterator<Key, Value>::operator++(int)
  {
    BSTConstIterator<Key, Value> tmp = *this;
    ++(*this);
    return tmp;
  }

  template <class Key, class Value>
  bool BSTConstIterator<Key, Value>::operator==(
      const BSTConstIterator<Key, Value> &other) const
  {
    return node == other.node;
  }

  template <class Key, class Value>
  bool BSTConstIterator<Key, Value>::operator!=(
      const BSTConstIterator<Key, Value> &other) const
  {
    return node != other.node;
  }

}

#endif
