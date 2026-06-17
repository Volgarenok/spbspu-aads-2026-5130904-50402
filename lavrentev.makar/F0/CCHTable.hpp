#ifndef CCHTABLE_HPP
#define CCHTABLE_HPP
#include <cstddef>
#include <algorithm>
#include <new>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace lavrentev
{
  template< class Key, class Value >
  struct Node
  {
    Key key;
    Value val;
    bool empty = true;
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
      void swap(CCHTable &other) noexcept;
      Value &operator[](const Key &k);

      void add(Key k, Value v);
      void drop(Key k);
      bool has(Key k) const;
      void rehash(size_t slots);
      size_t size() const;
      void print(std::ostream &os) const;
      size_t getCapacity() const;

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
  ht1(new Node[5]),
  ht2(new Node[5]),
  size1(0),
  cap1(5),
  size2(0),
  cap2(5),
  hasher(Hash{}),
  comp(Equal{}),
  loadFactor(0.5)
{}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >::CCHTable(const CCHTable &other):
  ht1(new Node[other.cap1]),
  ht2(new Node[other.cap2]),
  size1(other.size1),
  cap1(other.cap1),
  size2(other.size2),
  cap2(other.cap2),
  hasher(other.hasher),
  comp(other.comp),
  loadFactor(other.loadFactor)
{
  for (size_t i = 0; i < cap1; ++i)
  {
    ht1[i] = other.ht1[i];
  }
  for (size_t i = 0; i < cap2; ++i)
  {
    ht2[i] = other.ht2[i];
  }
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >::CCHTable(CCHTable &&other) noexcept:
  ht1(other.ht1),
  ht2(other.ht2),
  size1(other.size1),
  cap1(other.cap1),
  size2(other.size2),
  cap2(other.cap2),
  hasher(other.hasher),
  comp(other.comp),
  loadFactor(other.loadFactor)
{
  other.ht1 = nullptr;
  other.ht2 = nullptr;
  other.size1 = 0;
  other.cap1 = 0;
  other.size2 = 0;
  other.cap2 = 0;
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >::~CCHTable< Key, Value, Hash, Equal >() noexcept
{
  delete[] ht1;
  delete[] ht2;
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::CCHTable< Key, Value, Hash, Equal >
&lavrentev::CCHTable< Key, Value, Hash, Equal >::operator=(CCHTable other)
{
  swap(other);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void lavrentev::CCHTable< Key, Value, Hash, Equal >::swap(CCHTable &other) noexcept
{
  using std::swap;
  swap(ht1, other.ht1);
  swap(ht2, other.ht2);
  swap(size1, other.size1);
  swap(cap1, other.cap1);
  swap(size2, other.size2);
  swap(cap2, other.cap2);
  swap(hasher, other.hasher);
  swap(comp, other.comp);
  swap(loadFactor, other.loadFactor);
}

template< class Key, class Value, class Hash, class Equal >
Value &lavrentev::CCHTable< Key, Value, Hash, Equal >::operator[](const Key &k)
{
  size_t idx1 = hasher.hash1(k) % cap1;
  if (!ht1[idx1].empty && comp(ht1[idx1].key, k))
  {
    return ht1[idx1].val;
  }
  size_t idx2 = hasher.hash2(k) % cap2;
  if (!ht2[idx2].empty && comp(ht2[idx2].key, k))
  {
    return ht2[idx2].val;
  }
  add(k, Value());
  size_t new_idx1 = hasher.hash1(k) % cap1;
  if (!ht1[new_idx1].empty && comp(ht1[new_idx1].key, k))
  {
    return ht1[new_idx1].val;
  }
  size_t new_idx2 = hasher.hash2(k) % cap2;
  return ht2[new_idx2].val;
}

template< class Key, class Value, class Hash, class Equal >
void lavrentev::CCHTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (has(k))
  {
    return;
  }

  if ((size1 + size2) > (cap1 + cap2) * loadFactor)
  {
    rehash(cap1 * 2);
  }

  Key currK = std::move(k);
  Value currV = std::move(v);
  for (size_t i = 0; i < maxLoopCount; ++i) {
    size_t idx1 = hasher.hash1(currK) % cap1;
    if (ht1[idx1].empty)
    {
      ht1[idx1].key = std::move(currK);
      ht1[idx1].val = std::move(currV);
      ht1[idx1].empty = false;
      ++size1;
      return;
    }

    std::swap(currK, ht1[idx1].key);
    std::swap(currV, ht1[idx1].val);

    size_t idx2 = hasher.hash2(currK) % cap2;
    if (ht2[idx2].empty) {
      ht2[idx2].key = std::move(currK);
      ht2[idx2].val = std::move(currV);
      ht2[idx2].empty = false;
      ++size2;
      return;
    }

    std::swap(currK, ht2[idx2].key);
    std::swap(currV, ht2[idx2].val);
  }
  rehash(cap1 * 2);
  add(std::move(currK), std::move(currV));
}

template< class Key, class Value, class Hash, class Equal >
void lavrentev::CCHTable< Key, Value, Hash, Equal >::drop(Key k)
{
  size_t pos1 = hasher.hash1(k) % cap1;
  if (!ht1[pos1].empty && comp(ht1[pos1].key, k))
  {
    ht1[pos1].key = Key();
    ht1[pos1].val = Value();
    ht1[pos1].empty = true;
    --size1;
    return;
  }
  size_t pos2 = hasher.hash2(k) % cap2;
  if (!ht2[pos2].empty && comp(ht2[pos2].key, k))
  {
    ht2[pos2].key = Key();
    ht2[pos2].val = Value();
    ht2[pos2].empty = true;
    --size2;
    return;
  }
  throw std::out_of_range("Not such key");
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

template< class Key, class Value, class Hash, class Equal >
void lavrentev::CCHTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  if (slots < cap1)
  {
    throw std::invalid_argument("Not enough slots");
  }

  Node* oldht1 = ht1;
  Node* oldht2 = ht2;
  size_t oldcap1 = cap1;
  size_t oldcap2 = cap2;

  ht1 = new Node[slots];
  try
  {
    ht2 = new Node[slots];
  }
  catch (...)
  {
    delete[] ht1;
    ht1 = oldht1;
    throw std::bad_alloc();
  }

  cap1 = slots;
  cap2 = slots;
  size1 = 0;
  size2 = 0;

  for (size_t i = 0; i < oldcap1; ++i)
  {
    if (!oldht1[i].empty)
    {
      add(std::move(oldht1[i].key), std::move(oldht1[i].val));
    }
  }
  for (size_t i = 0; i < oldcap2; ++i)
  {
    if (!oldht2[i].empty)
    {
      add(std::move(oldht2[i].key), std::move(oldht2[i].val));
    }
  }

  delete[] oldht1;
  delete[] oldht2;
}

template< class Key, class Value, class Hash, class Equal >
size_t lavrentev::CCHTable<Key, Value, Hash, Equal >::size() const
{
  return size1 + size2;
}

template< class Key, class Value, class Hash, class Equal >
void lavrentev::CCHTable< Key, Value, Hash, Equal >::print(std::ostream &os) const
{
  for (size_t i = 0; i < cap1; ++i)
  {
    if (!ht1[i].empty)
    {
      os << ht1[i].key << ": " << ht1[i].val << '\n';
    }
  }

  for (size_t i = 0; i < cap2; ++i)
  {
    if (!ht2[i].empty)
    {
      os << ht2[i].key << ": " << ht2[i].val << '\n';
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
size_t lavrentev::CCHTable< Key, Value, Hash, Equal >::getCapacity() const
{
  return cap1 + cap2;
}

#endif
