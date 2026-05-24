#include <cstddef>
#include <utility>

namespace shirokov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class BSTIterator
  {
  public:
    explicit BSTIterator(typename BSTree< Key, Value, Compare >::Node*);
    bool operator==(const BSTIterator&) const noexcept;
    bool operator!=(const BSTIterator&) const noexcept;
    BSTIterator& operator++();
    std::pair< Key, Value > operator*() const;

  private:
    typename BSTree< Key, Value, Compare >::Node* curr;
  };

  template< class Key, class Value, class Compare >
  class BSTConstIterator
  {
  public:
    explicit BSTConstIterator(typename BSTree< Key, Value, Compare >::Node*);
    bool operator==(const BSTConstIterator&) const noexcept;
    bool operator!=(const BSTConstIterator&) const noexcept;
    BSTConstIterator& operator++();
    std::pair< Key, Value > operator*() const;

  private:
    const typename BSTree< Key, Value, Compare >::Node* curr;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    BSTree();
    BSTree(const BSTree&);
    BSTree(BSTree&&);
    ~BSTree();
    BSTree& operator=(const BSTree& rhs);
    BSTree& operator=(BSTree&& rhs);

    void push(Key k, Value v);
    Value get(Key k);
    bool drop(Key k);

    using const_iterator = BSTConstIterator< Key, Value, Compare >;
    using iterator = BSTIterator< Key, Value, Compare >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();

  private:
    friend iterator;
    friend const_iterator;
    struct Node
    {
      Key key;
      Value value;
      Node *lt, *rt, *parent;
    };
    Node* root;
  };
}
