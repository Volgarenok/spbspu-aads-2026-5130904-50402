#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <stdexcept>
#include <utility>
#include <functional>

namespace chernikov {

  template < typename Key, typename Value > class BSTIterator;

  template < typename Key, typename Value > class BSTConstIterator;

  template < typename Key, typename Value, typename Compare = std::less< Key > > class BSTree
  {
  public:
    using key_type = Key;
    using value_type = Value;
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

  private:
    struct Node
    {
      Key key;
      Value value;
      Node *left;
      Node *right;
      Node *parent;
      size_t height;

      Node(const Key &k, const Value &v, Node *p = nullptr):
        key(k),
        value(v),
        left(nullptr),
        right(nullptr),
        parent(p),
        height(1)
      {
      }
    };

    Node *root_;
    size_t size_;
    Compare comp_;

    void update_height(Node *node)
    {
      if (!node)
        return;
      size_t left_h = node->left ? node->left->height : 0;
      size_t right_h = node->right ? node->right->height : 0;
      node->height = 1 + (left_h > right_h ? left_h : right_h);
    }

    Node *find_node(const Key &k) const
    {
      Node *current = root_;
      while (current)
      {
        if (comp_(k, current->key))
        {
          current = current->left;
        } else if (comp_(current->key, k))
        {
          current = current->right;
        } else
        {
          return current;
        }
      }
      return nullptr;
    }

    Node *find_min(Node *node) const
    {
      if (!node)
        return nullptr;
      while (node->left)
      {
        node = node->left;
      }
      return node;
    }

    Node *find_max(Node *node) const
    {
      if (!node)
        return nullptr;
      while (node->right)
      {
        node = node->right;
      }
      return node;
    }

    void transplant(Node *old_node, Node *new_node)
    {
      if (!old_node->parent)
      {
        root_ = new_node;
      } else if (old_node == old_node->parent->left)
      {
        old_node->parent->left = new_node;
      } else
      {
        old_node->parent->right = new_node;
      }
      if (new_node)
      {
        new_node->parent = old_node->parent;
      }
    }

    void update_heights_up(Node *node)
    {
      while (node)
      {
        update_height(node);
        node = node->parent;
      }
    }

  public:
    BSTree():
      root_(nullptr),
      size_(0)
    {
    }

    ~BSTree()
    {
      clear();
    }

    BSTree(const BSTree &other):
      root_(nullptr),
      size_(0)
    {
      copy_from(other);
    }

    BSTree(BSTree &&other) noexcept:
      root_(other.root_),
      size_(other.size_)
    {
      other.root_ = nullptr;
      other.size_ = 0;
    }

    BSTree &operator=(const BSTree &other)
    {
      if (this != &other)
      {
        clear();
        copy_from(other);
      }
      return *this;
    }

    BSTree &operator=(BSTree &&other) noexcept
    {
      if (this != &other)
      {
        clear();
        root_ = other.root_;
        size_ = other.size_;
        other.root_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    void push(const Key &k, const Value &v)
    {
      Node *new_node = new Node(k, v);

      if (!root_)
      {
        root_ = new_node;
        size_ = 1;
        return;
      }

      Node *current = root_;
      Node *parent = nullptr;

      while (current)
      {
        parent = current;
        if (comp_(k, current->key))
        {
          current = current->left;
        } else if (comp_(current->key, k))
        {
          current = current->right;
        } else
        {
          current->value = v;
          delete new_node;
          return;
        }
      }

      new_node->parent = parent;
      if (comp_(k, parent->key))
      {
        parent->left = new_node;
      } else
      {
        parent->right = new_node;
      }
      ++size_;
      update_heights_up(parent);
    }

    Value &get(const Key &k)
    {
      Node *node = find_node(k);
      if (!node)
      {
        throw std::out_of_range("Key not found in BSTree::get()");
      }
      return node->value;
    }

    const Value &get(const Key &k) const
    {
      return const_cast< BSTree * >(this)->get(k);
    }

    Value drop(const Key &k)
    {
      Node *node = find_node(k);
      if (!node)
      {
        throw std::out_of_range("Key not found in BSTree::drop()");
      }

      Value removed = node->value;
      Node *parent = node->parent;

      if (!node->left)
      {
        transplant(node, node->right);
        update_heights_up(parent);
      } else if (!node->right)
      {
        transplant(node, node->left);
        update_heights_up(parent);
      } else
      {
        Node *successor = find_min(node->right);
        Node *succ_parent = successor->parent;

        if (successor->parent != node)
        {
          succ_parent = successor->parent;
          transplant(successor, successor->right);
          successor->right = node->right;
          if (successor->right)
          {
            successor->right->parent = successor;
          }
        }

        transplant(node, successor);
        successor->left = node->left;
        if (successor->left)
        {
          successor->left->parent = successor;
        }

        if (succ_parent == node)
        {
          update_heights_up(successor);
        } else
        {
          update_heights_up(succ_parent);
        }
      }

      delete node;
      --size_;
      return removed;
    }

    bool has(const Key &k) const
    {
      return find_node(k) != nullptr;
    }

    void clear()
    {
      clear_recursive(root_);
      root_ = nullptr;
      size_ = 0;
    }

    size_t size() const
    {
      return size_;
    }
    bool empty() const
    {
      return size_ == 0;
    }

    iterator begin()
    {
      return iterator(find_min(root_));
    }

    iterator end()
    {
      return iterator(nullptr);
    }

    const_iterator begin() const
    {
      return const_iterator(find_min(root_));
    }

    const_iterator end() const
    {
      return const_iterator(nullptr);
    }

    const_iterator rotateLeft(const_iterator it)
    {
      Node *node = const_cast< Node * >(it.node_);
      if (!node || !node->right)
        return it;

      Node *new_root = node->right;
      Node *parent = node->parent;

      node->right = new_root->left;
      if (new_root->left)
      {
        new_root->left->parent = node;
      }

      new_root->parent = parent;
      if (!parent)
      {
        root_ = new_root;
      } else if (node == parent->left)
      {
        parent->left = new_root;
      } else
      {
        parent->right = new_root;
      }

      new_root->left = node;
      node->parent = new_root;

      update_height(node);
      update_height(new_root);
      if (parent)
        update_height(parent);

      return const_iterator(node);
    }

    const_iterator rotateRight(const_iterator it)
    {
      Node *node = const_cast< Node * >(it.node_);
      if (!node || !node->left)
        return it;

      Node *new_root = node->left;
      Node *parent = node->parent;

      node->left = new_root->right;
      if (new_root->right)
      {
        new_root->right->parent = node;
      }

      new_root->parent = parent;
      if (!parent)
      {
        root_ = new_root;
      } else if (node == parent->left)
      {
        parent->left = new_root;
      } else
      {
        parent->right = new_root;
      }

      new_root->right = node;
      node->parent = new_root;

      update_height(node);
      update_height(new_root);
      if (parent)
        update_height(parent);

      return const_iterator(node);
    }

    const_iterator rotateLargeLeft(const_iterator it)
    {
      Node *node = const_cast< Node * >(it.node_);
      if (!node || !node->right)
        return it;
      if (!node->right->left)
        return rotateLeft(it);

      rotateRight(const_iterator(node->right));
      return rotateLeft(it);
    }

    const_iterator rotateLargeRight(const_iterator it)
    {
      Node *node = const_cast< Node * >(it.node_);
      if (!node || !node->left)
        return it;
      if (!node->left->right)
        return rotateRight(it);

      rotateLeft(const_iterator(node->left));
      return rotateRight(it);
    }

    size_t height(const_iterator it) const
    {
      if (!it.node_)
        return 0;
      return it.node_->height;
    }

    size_t height() const
    {
      return root_ ? root_->height : 0;
    }

    const_iterator find(const Key &k) const
    {
      return const_iterator(find_node(k));
    }

    iterator find(const Key &k)
    {
      return iterator(find_node(k));
    }

  private:
    void clear_recursive(Node *node)
    {
      if (!node)
        return;
      clear_recursive(node->left);
      clear_recursive(node->right);
      delete node;
    }

    void copy_from(const BSTree &other)
    {
      root_ = copy_recursive(other.root_, nullptr);
      size_ = other.size_;
    }

    Node *copy_recursive(Node *other_node, Node *parent)
    {
      if (!other_node)
        return nullptr;

      Node *new_node = new Node(other_node->key, other_node->value, parent);
      new_node->height = other_node->height;
      new_node->left = copy_recursive(other_node->left, new_node);
      new_node->right = copy_recursive(other_node->right, new_node);
      return new_node;
    }
  };

  template < typename Key, typename Value > class BSTIterator
  {
  public:
    friend class BSTree< Key, Value >;
    friend class BSTConstIterator< Key, Value >;

    using Node = typename BSTree< Key, Value >::Node;

  private:
    Node *node_;

    explicit BSTIterator(Node *node):
      node_(node)
    {
    }

  public:
    BSTIterator():
      node_(nullptr)
    {
    }
    BSTIterator(const BSTIterator &) = default;
    BSTIterator &operator=(const BSTIterator &) = default;

    std::pair< const Key &, Value & > operator*()
    {
      return {node_->key, node_->value};
    }

    BSTIterator &operator++()
    {
      if (!node_)
        return *this;

      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      } else
      {
        Node *parent = node_->parent;
        while (parent && node_ == parent->right)
        {
          node_ = parent;
          parent = parent->parent;
        }
        node_ = parent;
      }
      return *this;
    }

    BSTIterator operator++(int)
    {
      BSTIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTIterator &other) const
    {
      return node_ == other.node_;
    }
    bool operator!=(const BSTIterator &other) const
    {
      return !(*this == other);
    }
  };

  template < typename Key, typename Value > class BSTConstIterator
  {
  public:
    friend class BSTree< Key, Value >;

    using Node = typename BSTree< Key, Value >::Node;

    const Node *node_;

  private:
    explicit BSTConstIterator(const Node *node):
      node_(node)
    {
    }

  public:
    BSTConstIterator():
      node_(nullptr)
    {
    }
    BSTConstIterator(const BSTIterator< Key, Value > &it):
      node_(it.node_)
    {
    }
    BSTConstIterator(const BSTConstIterator &) = default;
    BSTConstIterator &operator=(const BSTConstIterator &) = default;

    std::pair< const Key &, const Value & > operator*() const
    {
      return {node_->key, node_->value};
    }

    BSTConstIterator &operator++()
    {
      if (!node_)
        return *this;

      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      } else
      {
        const Node *parent = node_->parent;
        while (parent && node_ == parent->right)
        {
          node_ = parent;
          parent = parent->parent;
        }
        node_ = parent;
      }
      return *this;
    }

    BSTConstIterator operator++(int)
    {
      BSTConstIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTConstIterator &other) const
    {
      return node_ == other.node_;
    }
    bool operator!=(const BSTConstIterator &other) const
    {
      return !(*this == other);
    }
  };

}

#endif
