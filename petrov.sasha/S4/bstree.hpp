#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <utility>

namespace petrov
{
  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using value_type = std::pair< const Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree();

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void swap(BSTree& other) noexcept;

    void push(const Key& key, const Value& value);
    Value& get(const Key& key);
    const Value& get(const Key& key) const;
    Value drop(const Key& key);

    bool has(const Key& key) const noexcept;
    bool empty() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    std::size_t height() const noexcept;
    std::size_t height(const_iterator it) const noexcept;

  private:
    struct Node
    {
      value_type data;
      Node* parent;
      Node* left;
      Node* right;

      Node(const Key& key, const Value& value, Node* parentNode);
    };

    Node* m_fake;
    Compare m_compare;

    Node* root() const noexcept;
    void setRoot(Node* node) noexcept;

    Node* findNode(const Key& key) const noexcept;
    Node* minimum(Node* node) const noexcept;

    void clear(Node* node) noexcept;
    Node* copy(Node* node, Node* parent);

    std::size_t height(Node* node) const noexcept;

    Node* rotateLeftNode(Node* node);
    Node* rotateRightNode(Node* node);

    void eraseNode(Node* node) noexcept;

    template< class K, class V >
    friend class BSTConstIterator;

    template< class K, class V >
    friend class BSTIterator;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    using value_type = std::pair< const Key, Value >;

    BSTConstIterator() noexcept;

    const value_type& operator*() const noexcept;
    const value_type* operator->() const noexcept;

    BSTConstIterator& operator++() noexcept;
    BSTConstIterator operator++(int) noexcept;

    bool operator==(const BSTConstIterator& other) const noexcept;
    bool operator!=(const BSTConstIterator& other) const noexcept;

  private:
    using Node = typename BSTree< Key, Value, std::less< Key > >::Node;

    Node* m_node;
    Node* m_fake;

    BSTConstIterator(Node* node, Node* fake) noexcept;

    template< class K, class V, class C >
    friend class BSTree;
    template< class K, class V >
    friend class BSTIterator;
  };

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    using value_type = std::pair< const Key, Value >;

    BSTIterator() noexcept;

    value_type& operator*() const noexcept;
    value_type* operator->() const noexcept;

    BSTIterator& operator++() noexcept;
    BSTIterator operator++(int) noexcept;

    bool operator==(const BSTIterator& other) const noexcept;
    bool operator!=(const BSTIterator& other) const noexcept;

    operator BSTConstIterator< Key, Value >() const noexcept;

  private:
    using Node = typename BSTree< Key, Value, std::less< Key > >::Node;

    Node* m_node;
    Node* m_fake;

    BSTIterator(Node* node, Node* fake) noexcept;

    template< class K, class V, class C >
    friend class BSTree;
  };
}

#endif


