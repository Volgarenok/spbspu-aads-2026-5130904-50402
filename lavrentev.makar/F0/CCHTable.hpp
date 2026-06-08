#ifndef CCHTABLE_HPP
#define CCHTABLE_HPP
#include <cstddef>
#include <algorithm>
#include <stdexcept>

namespace lavrentev
{
  template< class Key, class Value >
  struct Node
  {
    Key key;
    Value val;
    bool empty = true;
  };

  struct Audio
  {

  };

  template< class Key, class Value, class Hash, class Equal >
  struct CCHTable
  {
    using Node = lavrentev::Node< Key, Value >;
    public:
      CCHTable();
      CCHTable(const CCHTable &other);
      CCHTable(CCHTable &&other) noexcept;
      ~CCHTable() noexcept;
      CCHTable< Key, Value, Hash, Equal > &operator=(CCHTable other);
      Value &operator[](const Key &k);

      void add(Key k, Value v);
      void drop(Key k); //TODO
      bool has(Key k) const;
      void rehash(size_t slots); //TODO
      size_t size() const; //TODO

    private:
      Node *ht1;
      Node *ht2;
      size_t size1;
      size_t cap1;
      size_t size2;
      size_t cap2;
      Hash hasher;
      Equal comp;
      float loadFactor;
      const size_t maxLoopCount = 10;
  };
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >::CCHTable():
  ht1(new Node*[5]),
  ht2(new Node*[5]),
  size1(5),
  size2(5),
  hasher(Hash{}),
  comp(Equal{}),
  loadFactor(0.5)
{}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >::CCHTable(const CCHTable &other):
  ht1(new Node*[other.size1]),
  ht2(new Node*[other.size2]),
  size1(other.size1),
  size2(other.size2),
  hasher(other.hasher),
  comp(other.comp),
  loadFactor(other.loadFactor)
{
  for (size_t i = 0; i < size1; ++i)
  {
    ht1[i] = other.ht1[i];
  }
  for (size_t i = 0; i < size2; ++i)
  {
    ht2[i] = other.ht2[i];
  }
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >::CCHTable(CCHTable &&other) noexcept:
  ht1(new Node*[other.size1]),
  ht2(new Node*[other.size2]),
  size1(other.size1),
  size2(other.size2),
  hasher(other.hasher),
  comp(other.comp),
  loadFactor(other.loadFactor)
{
  other.ht1 = nullptr;
  other.ht2 = nullptr;
  other.size1 = 0;
  other.size2 = 0;
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >::~CCHTable() noexcept
{
  delete[] ht1;
  delete[] ht2;
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >
&lavrentev::CCHTable< Key, Value, Hash, Equal >::operator=(CCHTable other)
{
  if (other.ht1 == ht1 && other.ht2 == ht2)
  {
    return *this;
  }
  swap(other);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
Value &lavrentev::CCHTable< Key, Value, Hash, Equal >::operator[](const Key &k)
{

}

template< class Key, class Value, class Hash, class Equal >
void lavrentev::CCHTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (has(k))
  {
    return;
  }

  Key currK = std::move(k);
  Value currV = std::move(v);
  for (size_t i = 0; i < maxLoopCount; ++i) {
    size_t idx1 = hasher.hash1(currK) % cap1;
    if (ht1[idx1].empty)
    {
      ht1[idx1].key = std::move(currK);
      ht1[idx1].value = std::move(currV);
      ht1[idx1].empty = false;
      return;
    }

    std::swap(currK, ht1[idx1].key);
    std::swap(currV, ht1[idx1].value);

    size_t idx2 = hasher.hash2(currK) % cap2;
    if (ht2[idx2].empty) {
      ht2[idx2].key = std::move(currK);
      ht2[idx2].value = std::move(currV);
      ht2[idx2].empty = false;
      return;
    }

    std::swap(currK, ht2[idx2].key);
    std::swap(currV, ht2[idx2].value);
  }
  rehash(size1 * 2);
  add(std::move(currK), std::move(currV));
}

template< class Key, class Value, class Hash, class Equal >
bool lavrentev::CCHTable< Key, Value, Hash, Equal >::has(Key k) const
{
  size_t pos1 = hasher.hash1(k) % cap1;
  if (!ht1[pos1].empty && comp(ht1[pos1].key, k))
  {
    return true;
  }

  size_t pos2 = hasher.hash2(k) % cap2;
  if (!ht2[pos2].empty && comp(ht2[pos2].key, k))
  {
    return true;
  }

  return false;
}

#endif
