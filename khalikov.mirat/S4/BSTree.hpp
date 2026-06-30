#ifndef BSTREE_HPP
#define BSTREE_HPP

template< class Key, class Value, class Compare >
class BSTree
{
public:
  BSTree();
  BSTree(const BSTree &other);
  BSTree(BSTree &&other) noexcept;
  ~BSTree();

  BSTree &operator=(const BSTree &rhs);
  BSTree &operator=(BSTree &&rhs);

  void swap(BSTree &other) noexcept;
private:
  struct Node
  {
    Key key;
    Value value;
    Node *left, *right, *parent;
  }

  Node *fakeRoot_;
  Node *nil_;
  Compare cmp_;
};
