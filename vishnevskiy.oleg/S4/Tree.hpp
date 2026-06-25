#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <stdexcept>

namespace vishnevskiy
{
  template <class Key, class Value, class Compare>
  class BSTree;

  template <class Key, class Value>
  struct Node;

  template <class Key, class Value>
  class BSIterator
  {
    private:
      Node<Key, Value>* curr;
    public:
      BSIterator(Node<Key, Value>* node);
      void next();
      void prev();
      void fallLeft();
      void fallRight();
      Value* getVal();
      Key* getKey();
      Node<Key, Value>* getNode();
      bool isEnd() const;
  };

  template <class Key, class Value>
  class BSConstIterator
  {
    private:
      Node<Key, Value>* curr;
    public:
      BSConstIterator(const Node<Key, Value>* node);
      void next();
      void prev();
      void fallLeft();
      void fallRight();
      const Value* getVal() const;
      const Key* getKey() const;
      const Node<Key, Value>* getNode() const;
      bool isEnd() const;
  };

  template <class Key, class Value>
  struct Node
  {
    private:
      Key* key;
      Value* val;
      Node* parent;
      Node* left;
      Node* right;
      size_t height;
    public:
      Node();
      Node(Key* k, Value* v, Node* p);
      ~Node();
      bool isLeaf() const;
      template <class K, class V, class C>
      friend class BSTree;
      friend class BSIterator<Key, Value>;
      friend class BSConstIterator<Key, Value>;
  };

  template <class Key, class Value, class Compare>
  class BSTree
  {
    private:
      Node<Key, Value>* fakeRoot;
      Node<Key, Value>* fakeLeaf;
      Compare cmp;
      size_t nodes;
      Node<Key, Value>* findByKey(const Key& k) const;
      void updateHeight(Node<Key, Value>* node);
      void destroyTree(Node<Key, Value>* node);
      Node<Key, Value>* copy(Node<Key, Value>* node,Node<Key, Value>* parent);
    public:
      BSTree(Compare c);
      ~BSTree();
      void clear();
      BSTree(const BSTree& other);
      BSTree& operator=(const BSTree& other);
      BSIterator<Key, Value> begin();
      BSConstIterator<Key, Value> beginConst() const;
      bool has(const Key& k) const;
      void push(const Key& k, const Value& v);
      int getBalance(Node<Key, Value>* node);
      Value get(const Key& k);
      Value drop(const Key& k);

      using iterator = BSIterator<Key, Value>;
      iterator rotateLeft(iterator it);
      iterator rotateRight(iterator it);
      iterator rotateLargeLeft(iterator it);
      iterator rotateLargeRight(iterator it);
      int height(iterator it) const;
      int height() const;
  };
}

#endif
