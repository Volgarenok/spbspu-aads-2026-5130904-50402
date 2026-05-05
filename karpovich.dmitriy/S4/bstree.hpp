#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include "bstiterators.hpp"
#include "treenode.hpp"

namespace karpovich
{
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree &other);
    BSTree(BSTree &&other);
    ~BSTree();

    BSTree &operator=(const BSTree &other);
    BSTree &operator=(BSTree &&other);

    bool empty() const;
    size_t size() const;

    const Value &at(const Key &k) const;
    Value &at(const Key &k);

    void push(const Key &k, const Value &v);
    void push(Key &&k, Value &&v);
    Value drop(const Key &k);

    void clear();
    void swap(BSTree &other);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height() const;
    size_t height(const_iterator it) const;

    friend class BSTConstIterator< Key, Value >;
    friend class BSTIterator< Key, Value >;

  private:
    TreeNode< Key, Value > *root_;
    size_t size_;
    Compare comp_;
  };
}

#endif
