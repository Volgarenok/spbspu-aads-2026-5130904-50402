#ifndef TREEIMPL_HPP
#define TREEIMPL_HPP

#include <cstddef>
#include <stdexcept>
#include <iostream>

#include "Tree.hpp"

namespace vishnevskiy
{
  template <class Key, class Value>
  Node<Key, Value>::Node():
    key(nullptr),
    val(nullptr),
    parent(nullptr),
    left(nullptr),
    right(nullptr),
    height(0)
  {}

  template <class Key, class Value>
  Node<Key, Value>::Node(Key* k, Value* v, Node* p):
    key(k),
    val(v),
    parent(p),
    left(nullptr),
    right(nullptr),
    height(1)
  {}

  template <class Key, class Value>
  Node<Key, Value>::~Node()
  {
    delete key;
    delete val;
  }

  template <class Key, class Value>
  bool Node<Key, Value>::isLeaf() const {
    return key == nullptr && val == nullptr && left == this && right == this;
  }

  template <class Key, class Value>
  BSIterator<Key, Value>::BSIterator(Node<Key, Value>* node):
    curr(node)
  {}

  template <class Key, class Value, class Compare>
  BSIterator<Key, Value> BSTree<Key, Value, Compare>::begin()
  {
    Node<Key, Value>* curr = fakeRoot -> right;
    if (curr && !curr -> isLeaf())
    {
      while (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
      }
    }
    return BSIterator<Key, Value>(curr);
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::fallRight()
  {
    if (curr && !(curr -> isLeaf()))
    {
      while (curr -> right && !curr -> right -> isLeaf())
      {
        curr = curr -> right;
      }
    }
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::fallLeft()
  {
    if (curr && !(curr -> isLeaf()))
    {
      while (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
      }
    }
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::next()
  {
    if (curr && !(curr -> isLeaf()))
    {
      if (curr -> right && !curr -> right -> isLeaf())
      {
        curr = curr -> right;
        fallLeft();
        return;
      }
      Node<Key, Value>* parent = curr -> parent;
      while (parent && curr == parent -> right && !parent -> isLeaf())
      {
        curr = parent;
        parent = parent -> parent;
      }
      curr = parent;
    }
  }

  template <class Key, class Value>
  void BSIterator<Key, Value>::prev()
  {
    if (curr && !(curr -> isLeaf()))
    {
      if (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
        fallRight();
        return;
      }
      Node<Key, Value>* parent = curr -> parent;
      while (parent && curr == parent -> left)
      {
        curr = parent;
        parent = parent -> parent;
      }
      curr = parent;
    }
  }

  template <class Key, class Value>
  Value* BSIterator<Key, Value>::getVal()
  {
    if (curr)
    {
      return curr -> val;
    }
    return nullptr;
  }

  template <class Key, class Value>
  Key* BSIterator<Key, Value>::getKey()
  {
    if (curr)
    {
      return curr -> key;
    }
    return nullptr;
  }

  template <class Key, class Value>
  Node<Key, Value>* BSIterator<Key, Value>::getNode()
  {
    return curr;
  }

  template <class Key, class Value>
  bool BSIterator<Key, Value>::isEnd() const
  {
    return !curr || curr->isLeaf();
  }

  template <class Key, class Value>
  BSConstIterator<Key, Value>::BSConstIterator(const Node<Key, Value>* node):
    curr(node)
  {}

  template <class Key, class Value, class Compare>
  BSConstIterator<Key, Value> BSTree<Key, Value, Compare>::beginConst() const
  {
    const Node<Key, Value>* curr = fakeRoot -> right;
    if (curr && !curr -> isLeaf())
    {
      while (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
      }
    }
    return BSConstIterator<Key, Value>(curr);
  }

  template <class Key, class Value>
  void BSConstIterator<Key, Value>::fallRight()
  {
    if (curr && !(curr -> isLeaf()))
    {
      while (curr -> right && !curr -> right -> isLeaf())
      {
        curr = curr -> right;
      }
    }
  }

  template <class Key, class Value>
  void BSConstIterator<Key, Value>::fallLeft()
  {
    if (curr && !(curr -> isLeaf()))
    {
      while (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
      }
    }
  }

  template <class Key, class Value>
  void BSConstIterator<Key, Value>::next()
  {
    if (curr && !(curr -> isLeaf()))
    {
      if (curr -> right && !curr -> right -> isLeaf())
      {
        curr = curr -> right;
        fallLeft();
        return;
      }
      Node<Key, Value>* parent = curr -> parent;
      while (parent && curr == parent -> right)
      {
        curr = parent;
        parent = parent -> parent;
      }
      curr = parent;
    }
  }

  template <class Key, class Value>
  void BSConstIterator<Key, Value>::prev()
  {
    if (curr && !(curr -> isLeaf()))
    {
      if (curr -> left && !curr -> left -> isLeaf())
      {
        curr = curr -> left;
        fallRight();
        return;
      }
      Node<Key, Value>* parent = curr -> parent;
      while (parent && curr == parent -> left)
      {
        curr = parent;
        parent = parent -> parent;
      }
      curr = parent;
    }
  }

  template <class Key, class Value>
  const Value* BSConstIterator<Key, Value>::getVal() const
  {
    if (curr)
    {
      return curr -> val;
    }
  }

  template <class Key, class Value>
  const Key* BSConstIterator<Key, Value>::getKey() const
  {
    if (curr)
    {
      return curr -> key;
    }
  }

  template <class Key, class Value>
  const Node<Key, Value>* BSConstIterator<Key, Value>::getNode() const
  {
    return curr;
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::destroyTree(Node<Key, Value>* node)
  {
    if (node && !(node->isLeaf()))
    {
      destroyTree(node->left);
      destroyTree(node->right);
      delete node;
    }
  }

  template <class Key, class Value>
  bool BSConstIterator<Key, Value>::isEnd() const
  {
    return !curr || curr->isLeaf();
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree(Compare c):
    cmp(c),
    nodes(0)
  {
    fakeLeaf = new Node<Key, Value>();
    fakeLeaf -> left = fakeLeaf;
    fakeLeaf -> right = fakeLeaf;
    fakeLeaf -> parent = fakeLeaf;
    fakeLeaf -> height = 0;
    fakeRoot = new Node<Key, Value>();
    fakeRoot -> key = nullptr;
    fakeRoot -> val = nullptr;
    fakeRoot -> left = fakeLeaf;
    fakeRoot -> right = fakeLeaf;
    fakeRoot -> parent = nullptr;
    fakeRoot -> height = 0;
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::~BSTree()
  {
    clear();
    delete fakeRoot;
    delete fakeLeaf;
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::clear()
  {
    BSIterator<Key, Value> it = begin();
    while (!it.isEnd())
    {
      delete it.getVal();
      it.getNode()->val = nullptr;
      it.next();
    }
    destroyTree(fakeRoot->right);
    fakeRoot->right = fakeLeaf;
    nodes = 0;
  }

  template <class Key, class Value, class Compare>
  Node<Key, Value>* BSTree<Key, Value, Compare>::copy(Node<Key, Value>* node,Node<Key, Value>* parent)
  {
    if (!node || node -> isLeaf())
    {
      return fakeLeaf;
    }

    Node<Key, Value>* n = new Node<Key, Value>(new Key(*(node -> key)), new Value(*(node -> val)), parent);
    n -> height = node -> height;
    n -> left = copy(node -> left, n);
    n -> right = copy(node -> right, n);
    return n;
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree(const BSTree& other):
    cmp(other.cmp),
    nodes(0)
  {
    fakeLeaf = new Node<Key, Value>();
    fakeLeaf -> left = fakeLeaf;
    fakeLeaf -> right = fakeLeaf;
    fakeLeaf -> parent = fakeLeaf;
    fakeLeaf -> height = 0;
    fakeRoot = new Node<Key, Value>();
    fakeRoot -> key = nullptr;
    fakeRoot -> val = nullptr;
    fakeRoot -> left = fakeLeaf;
    fakeRoot -> right = fakeLeaf;
    fakeRoot -> parent = nullptr;
    fakeRoot -> height = 0;
    if (other.fakeRoot -> right && !other.fakeRoot -> right -> isLeaf())
    {
      fakeRoot -> right = copy(other.fakeRoot -> right, fakeRoot);
      nodes = other.nodes;
    }
  }

  template <class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>& BSTree<Key, Value, Compare>::operator=(const BSTree& other)
  {
    if (this != &other)
    {
      cmp = other.cmp;
      Node<Key, Value>* curr = fakeRoot -> right;
      if (curr && !curr -> isLeaf())
      {
        while (!curr -> isLeaf())
        {
          if (curr -> left && !curr -> left -> isLeaf())
          {
            curr = curr -> left;
          }
          else if (curr->right && !curr -> right -> isLeaf())
          {
            curr = curr -> right;
          }
          else
          {
            Node<Key, Value>* parent = curr -> parent;
            if (parent -> left == curr)
            {
              parent -> left = fakeLeaf;
            }
            else if (parent -> right == curr)
            {
              parent -> right = fakeLeaf;
            }
            delete curr;
            curr = parent;
            if (curr == fakeRoot)
            {
              break;
            }
          }
        }
      }
      fakeRoot -> right = fakeLeaf;
      nodes = 0;
      if (other.fakeRoot -> right && !other.fakeRoot -> right -> isLeaf())
      {
        fakeRoot -> right = copy(other.fakeRoot -> right, fakeRoot);
        nodes = other.nodes;
      }
    }
    return *this;
  }

  template <class Key, class Value, class Compare>
  Node<Key, Value>* BSTree<Key, Value, Compare>::findByKey(const Key& k) const
  {
    Node<Key, Value>* curr = fakeRoot -> right;
    while (curr && !curr -> isLeaf())
    {
      if (curr->key == nullptr)
      {
        return fakeLeaf;
      }
      if (cmp(k, *(curr->key)))
      {
        curr = curr -> left;
      }
      else if (cmp(*(curr -> key), k))
      {
        curr = curr -> right;
      }
      else
      {
        return curr;
      }
    }
    return fakeLeaf;
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::updateHeight(Node<Key, Value>* node)
  {
    if (node && !(node -> isLeaf()))
    {
      int heightL = 0;
      int heightR = 0;
      if (node -> left && !node -> left -> isLeaf())
      {
        heightL = node -> left -> height;
      }
      if (node -> right && !node -> right -> isLeaf())
      {
        heightR = node -> right -> height;
      }
      if (heightL > heightR)
      {
        node -> height = 1 + heightL;
      }
      else
      {
        node -> height = 1 + heightR;
      }
    }
  }

  template <class Key, class Value, class Compare>
  int BSTree<Key, Value, Compare>::getBalance(Node<Key, Value>* node)
  {
    if (node && !(node -> isLeaf()))
    {
      int heightL = 0;
      int heightR = 0;
      if (node -> left && !node -> left -> isLeaf())
      {
        heightL = node -> left -> height;
      }
      if (node -> right && !node -> right -> isLeaf())
      {
        heightR = node -> right -> height;
      }
      return heightL - heightR;
    }
    throw std::logic_error("No node found");
  }

  template <class Key, class Value, class Compare>
  BSIterator<Key, Value> BSTree<Key, Value, Compare>::rotateLeft(BSIterator<Key, Value> it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    Node<Key, Value>* rotChild = rotParent -> right;
    if (!rotChild || rotChild -> isLeaf())
    {
      return it;
    }
    Node<Key, Value>* oldLeft = rotChild->left;
    rotChild->parent = rotParent->parent;
    rotChild->left = rotParent;
    rotParent->parent = rotChild;
    rotParent->right = oldLeft;
    if (oldLeft && !oldLeft->isLeaf())
    {
      oldLeft->parent = rotParent;
    }
    updateHeight(rotParent);
    updateHeight(rotChild);
    if (rotParent == fakeRoot -> right)
    {
      fakeRoot -> right = rotChild;
    }
    else if (rotChild -> parent -> left == rotParent)
    {
      rotChild -> parent -> left = rotChild;
    }
    else
    {
      rotChild -> parent -> right = rotChild;
    }
    return BSIterator<Key, Value>(rotChild);
  }

  template <class Key, class Value, class Compare>
  BSIterator<Key, Value> BSTree<Key, Value, Compare>::rotateRight(BSIterator<Key, Value> it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    Node<Key, Value>* rotChild = rotParent -> left;
    if (!rotChild || rotChild -> isLeaf())
    {
      return it;
    }
    Node<Key, Value>* oldRight = rotChild->right;
    rotChild->parent = rotParent->parent;
    rotChild->right = rotParent;
    rotParent->parent = rotChild;
    rotParent->left = oldRight;
    if (oldRight && !oldRight->isLeaf())
    {
      oldRight->parent = rotParent;
    }
    updateHeight(rotParent);
    updateHeight(rotChild);
    if (rotParent == fakeRoot -> right)
    {
      fakeRoot -> right = rotChild;
    }
    else if (rotChild -> parent -> left == rotParent)
    {
      rotChild -> parent -> left = rotChild;
    }
    else
    {
      rotChild -> parent -> right = rotChild;
    }
    return BSIterator<Key, Value>(rotChild);
  }

  template <class Key, class Value, class Compare>
  BSIterator<Key, Value> BSTree<Key, Value, Compare>::rotateLargeLeft(BSIterator<Key, Value> it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    BSIterator<Key, Value> newLeft = rotateLeft(BSIterator<Key, Value>(rotParent -> left));
    rotParent -> left = newLeft.getNode();
    if (rotParent -> left && !rotParent -> left -> isLeaf())
    {
      rotParent -> left -> parent = rotParent;
    }
    return rotateRight(BSIterator<Key, Value>(rotParent));
  }

  template <class Key, class Value, class Compare>
  BSIterator<Key, Value> BSTree<Key, Value, Compare>::rotateLargeRight(BSIterator<Key, Value> it)
  {
    Node<Key, Value>* rotParent = it.getNode();
    if (!rotParent || rotParent -> isLeaf())
    {
      return it;
    }
    BSIterator<Key, Value> newRight = rotateRight(BSIterator<Key, Value>(rotParent -> right));
    rotParent -> right = newRight.getNode();
    if (rotParent -> right && !rotParent -> right -> isLeaf())
    {
      rotParent -> right -> parent = rotParent;
    }
    return rotateLeft(BSIterator<Key, Value>(rotParent));
  }

  template <class Key, class Value, class Compare>
  bool BSTree<Key, Value, Compare>::has(const Key& k) const
  {
    return findByKey(k) != fakeLeaf;
  }

  template <class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::push(const Key& k, const Value& v)
  {
    Node<Key, Value>* curr = fakeRoot->right;
    Node<Key, Value>* par = fakeRoot;
    while (!curr->isLeaf())
    {
      par = curr;
      if (cmp(k, *(curr->key)))
      {
        curr = curr->left;
      }
      else if (cmp(*(curr->key), k))
      {
        curr = curr->right;
      }
      else
      {
        delete curr->val;
        curr->val = new Value(v);
        return;
      }
    }
    Key* newKey = new Key(k);
    Value* newVal = new Value(v);
    Node<Key, Value>* newNode = new Node<Key, Value>(newKey, newVal, par);
    newNode->left = fakeLeaf;
    newNode->right = fakeLeaf;
    if (par == fakeRoot)
    {
      fakeRoot->right = newNode;
    }
    else if (cmp(k, *(par->key)))
    {
      par->left = newNode;
    }
    else
    {
      par->right = newNode;
    }
    nodes++;
    Node<Key, Value>* newPar = newNode->parent;
    while (newPar != fakeRoot)
    {
      updateHeight(newPar);
      int balance = getBalance(newPar);
      if (balance > 1)
      {
        if (getBalance(newPar->left) >= 0)
        {
          rotateRight(BSIterator<Key, Value>(newPar));
        }
        else
        {
          rotateLargeLeft(BSIterator<Key, Value>(newPar));
        }
        break;
      }
      else if (balance < -1)
      {
        if (getBalance(newPar->right) <= 0)
        {
          rotateLeft(BSIterator<Key, Value>(newPar));
        }
        else
        {
          rotateLargeRight(BSIterator<Key, Value>(newPar));
        }
        break;
      }
      newPar = newPar->parent;
    }
  }

  template <class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::get(const Key& k)
  {
    Node<Key, Value>* node = findByKey(k);
    if (node == fakeLeaf)
    {
      throw std::logic_error("Key not found");
    }
    return *(node->val);
  }

  template <class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::drop(const Key& k)
  {
    Node<Key, Value>* node = findByKey(k);
    if (node == fakeLeaf)
    {
      throw std::logic_error("Key not found");
    }
    Value val = *(node->val);
    if (!node -> left -> isLeaf() && !node -> right -> isLeaf())
    {
      Node<Key, Value>* nextNode = node -> right;
      while (!nextNode -> left -> isLeaf())
      {
        nextNode = nextNode -> left;
      }
      Key* tempKey = node -> key;
      Value* tempVal = node -> val;
      node -> key = nextNode -> key;
      node -> val = nextNode -> val;
      nextNode -> key = tempKey;
      nextNode -> val = tempVal;
      Node<Key, Value>* toDelete = nextNode;
      Node<Key, Value>* newLeaf;
      if (!toDelete -> left -> isLeaf())
      {
        newLeaf = toDelete -> left;
      }
      else
      {
        newLeaf = toDelete -> right;
      }
      newLeaf -> parent = toDelete -> parent;

      if (toDelete -> parent -> left == toDelete)
      {
        toDelete -> parent -> left = newLeaf;
      }
      else
      {
        toDelete -> parent -> right = newLeaf;
      }
      delete toDelete;
      nodes--;
      Node<Key, Value>* newPar = newLeaf -> parent;
      while (newPar && newPar != fakeRoot)
      {
        updateHeight(newPar);
        int balance = getBalance(newPar);
        if (balance > 1)
        {
          if (getBalance(newPar -> left) >= 0)
          {
            rotateRight(BSIterator<Key, Value>(newPar));
          }
          else
          {
            rotateLargeLeft(BSIterator<Key, Value>(newPar));
          }
        }
        else if (balance < -1)
        {
          if (getBalance(newPar -> right) <= 0)
          {
            rotateLeft(BSIterator<Key, Value>(newPar));
          }
          else
          {
            rotateLargeRight(BSIterator<Key, Value>(newPar));
          }
        }
        newPar = newPar->parent;
      }
      return val;
    }
    Node<Key, Value>* newLeaf;
    if (!node -> left -> isLeaf())
    {
      newLeaf = node -> left;
    }
    else
    {
      newLeaf = node -> right;
    }
    newLeaf -> parent = node -> parent;
    if (node -> parent -> left == node)
    {
      node -> parent -> left = newLeaf;
    }
    else
    {
      node -> parent -> right = newLeaf;
    }
    if (node == fakeRoot -> right)
    {
      fakeRoot -> right = newLeaf;
    }
    delete node;
    nodes--;
    Node<Key, Value>* newPar = newLeaf -> parent;
    while (newPar && newPar != fakeRoot)
    {
      updateHeight(newPar);
      int balance = getBalance(newPar);
      if (balance > 1)
      {
        if (getBalance(newPar -> left) >= 0)
        {
          rotateRight(BSIterator<Key, Value>(newPar));
        }
        else
        {
          rotateLargeLeft(BSIterator<Key, Value>(newPar));
        }
      }
      else if (balance < -1)
      {
        if (getBalance(newPar -> right) <= 0)
        {
          rotateLeft(BSIterator<Key, Value>(newPar));
        }
        else
        {
          rotateLargeRight(BSIterator<Key, Value>(newPar));
        }
      }
      newPar = newPar -> parent;
    }
    return val;
  }

  template <class Key, class Value, class Compare>
  int BSTree<Key, Value, Compare>::height() const
  {
    if (fakeRoot -> right -> isLeaf())
    {
      return 0;
    }
    return fakeRoot -> right -> height;
  }

  template <class Key, class Value, class Compare>
  int BSTree<Key, Value, Compare>::height(BSIterator<Key, Value> it) const
  {
    const Node<Key, Value>* node = it.getNode();
    if (!node || node -> isLeaf())
    {
      return 0;
    }
    return node -> height;
  }
}

#endif
