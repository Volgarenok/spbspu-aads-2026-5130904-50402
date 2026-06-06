#ifndef CCHTABLE_HPP
#define CCHTABLE_HPP
#include <cstddef>

namespace lavrentev
{
  template< class Key, class Value >
  struct Node
  {
    Key key_;
    Value val_;
    bool busy_ = false;
  };

  template< class Key, class Value >
  struct CCHTable
  {
    using Node = lavrentev::Node< Key, Value >;
    public:
      CCHTable();
      CCHTable(const CCHTable &);
      CCHTable(CCHTable &&) noexcept;
      ~CCHTable() noexcept;
      CCHTable< Key, Value > &operator=(CCHTable< Key, Value >);
      Value &operator[](const Key &k);

      void add(Key k, Value v);
      void drop(Key k);
      bool has(Key k) const;
      void rehash(size_t slots);
      size_t size() const;

    private:
      Node *ht1;
      Node *ht2;
      double loadFactor_;
  };
}

#endif
