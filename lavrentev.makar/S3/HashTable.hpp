#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <cstddef>
#include <stdexcept>
#include <List.hpp>
#include "siphash/hasher.hpp"

namespace lavrentev
{
  template <class Key, class Value, class Hash, class Equal> class HashTable;

  template <class Key, class Value, class Hash, class Equal> class HashIter;
  template <class Key, class Value, class Hash, class Equal> class HashCIter;

  template <class Key, class Value, class Hash, class Equal> class HashTable
  {
  private:
    struct Node
    {
      Key key;
      Value value;
    };
    List<Node> *ht_;
    std::size_t slots_;
    std::size_t size_;
    double loadFactor_;
    Hash hasher_;
    Equal equal_;
    void swap(HashTable<Key, Value, Hash, Equal>& other) noexcept;

  public:
    friend class HashIter<Key, Value, Hash, Equal>;
    friend class HashCIter<Key, Value, Hash, Equal>;
    friend struct Graph;

    HashTable();
    HashTable(const HashTable &);
    HashTable(HashTable &&) noexcept;
    ~HashTable() noexcept;
    HashTable<Key, Value, Hash, Equal> &
    operator=(HashTable<Key, Value, Hash, Equal>);
    Value &operator[](const Key &k);

    void add(Key k, Value v);
    void drop(Key k);
    bool has(Key k) const;
    void rehash(size_t slots);
    size_t size() const;
    size_t bucket_count() const;
  };

  template <class Key, class Value, class Hash, class Equal> class HashIter
  {
  private:
    using Node = typename HashTable<Key, Value, Hash, Equal>::Node;
    List<Node> *bucket_ptr_;
    List<Node> *bucket_end_;
    LIter<Node> list_iter_;

    void next();

  public:
    HashIter(List<Node> *ptr, List<Node> *end, LIter<Node> lit);

    bool operator==(const HashIter<Key, Value, Hash, Equal> &other) const;
    bool operator!=(const HashIter<Key, Value, Hash, Equal> &other) const;
    HashIter<Key, Value, Hash, Equal> &operator++();
    Node &operator*();
  };

  template <class Key, class Value, class Hash, class Equal> class HashCIter
  {
  private:
    using Node = typename HashTable<Key, Value, Hash, Equal>::Node;
    List<Node> *bucket_ptr_;
    List<Node> *bucket_end_;
    LCIter<Node> list_iter_;

    void next();

  public:
    HashCIter(List<Node> *ptr, List<Node> *end, LCIter<Node> lit);

    bool operator==(const HashCIter<Key, Value, Hash, Equal> &other) const;
    bool operator!=(const HashCIter<Key, Value, Hash, Equal> &other) const;
    HashCIter<Key, Value, Hash, Equal> &operator++();
    const Node &operator*() const;
  };
} // namespace lavrentev

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>::HashTable()
    : ht_(new List<Node>[5]), slots_(5), size_(0), loadFactor_(0.7)
{
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>::~HashTable() noexcept
{
  delete[] ht_;
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable &other)
    : ht_(new List<Node>[other.slots_]), slots_(other.slots_),
      size_(other.size_), hasher_(other.hasher_), equal_(other.equal_)
{
  for (size_t i = 0; i < slots_; ++i)
  {
    ht_[i] = other.ht_[i];
  }
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>::HashTable(
    HashTable &&other) noexcept
    : ht_(other.ht_), slots_(other.slots_), size_(other.size_),
      hasher_(std::move(other.hasher_)), equal_(std::move(other.equal_))
{
  other.ht_ = nullptr;
  other.slots_ = 0;
  other.size_ = 0;
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>&
lavrentev::HashTable<Key, Value, Hash, Equal>::operator=(HashTable other)
{
  swap(other);
  return *this;
}

template <class Key, class Value, class Hash, class Equal>
Value &lavrentev::HashTable<Key, Value, Hash, Equal>::operator[](const Key &k)
{
  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (Equal{}((*it).key, k))
    {
      return (*it).value;
    }
  }
  bucket.pushFront(Node{k, Value{}});
  ++size_;
  return bucket.front().value;
}

template <class Key, class Value, class Hash, class Equal>
void lavrentev::HashTable<Key, Value, Hash, Equal>::add(Key k, Value v)
{
  if (slots_ == 0)
  {
    throw std::invalid_argument("Not enougth slots");
  }
  if ((size_ + 1.0) / slots_ > loadFactor_)
  {
    throw std::overflow_error("Overflow error");
  }

  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (equal_((*it).key, k))
    {
      throw std::invalid_argument("Key already exists");
    }
  }
  bucket.pushFront(Node{k, v});
  ++size_;
}

template <class Key, class Value, class Hash, class Equal>
void lavrentev::HashTable<Key, Value, Hash, Equal>::drop(Key k)
{
  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (equal_((*it).key, k))
    {
      Value out = (*it).value;
      bucket.erase(it);
      --size_;
    }
  }
  throw std::invalid_argument("Key is not exists");
}

template <class Key, class Value, class Hash, class Equal>
bool lavrentev::HashTable<Key, Value, Hash, Equal>::has(Key k) const
{
  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (equal_((*it).key, k))
    {
      return true;
    }
  }
  return false;
}

template <class Key, class Value, class Hash, class Equal>
void lavrentev::HashTable<Key, Value, Hash, Equal>::rehash(size_t slots)
{
  if (slots == 0)
  {
    throw std::invalid_argument("Amount of slots must be positive");
  }
  List<Node> *new_ht = new List<Node>[slots];
  for (size_t i = 0; i < slots_; ++i)
  {
    for (LIter<Node> it = ht_[i].begin(); it != ht_[i].end(); ++it)
    {
      size_t new_idx = hasher_((*it).key) % slots;
      new_ht[new_idx].pushFront(Node{(*it).key, (*it).value});
    }
  }
  delete[] ht_;
  ht_ = new_ht;
  slots_ = slots;
}

template <class Key, class Value, class Hash, class Equal>
size_t lavrentev::HashTable<Key, Value, Hash, Equal>::size() const
{
  return size_;
}

template <class Key, class Value, class Hash, class Equal>
size_t lavrentev::HashTable<Key, Value, Hash, Equal>::bucket_count() const
{
  return slots_;
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashIter<Key, Value, Hash, Equal>::HashIter(
  List<Node> *ptr,
  List<Node> *end,
  LIter<Node> lit)
    : bucket_ptr_(ptr), bucket_end_(end), list_iter_(lit)
{
  if (bucket_ptr_ != bucket_end_ && list_iter_ == (*bucket_ptr_).end())
  {
    next();
  }
}

template <class Key, class Value, class Hash, class Equal>
bool lavrentev::HashIter<Key, Value, Hash, Equal>::operator==(
    const HashIter<Key, Value, Hash, Equal> &other) const
{
  return list_iter_ == other.list_iter_ && bucket_ptr_ == other.bucket_ptr_;
}

template <class Key, class Value, class Hash, class Equal>
bool lavrentev::HashIter<Key, Value, Hash, Equal>::operator!=(
    const HashIter<Key, Value, Hash, Equal> &other) const
{
  return !(*this == other);
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashIter<Key, Value, Hash, Equal> &
lavrentev::HashIter<Key, Value, Hash, Equal>::operator++()
{
  ++list_iter_;
  next();
  return *this;
}

template <class Key, class Value, class Hash, class Equal>
typename lavrentev::HashTable<Key, Value, Hash, Equal>::Node &lavrentev::HashIter<Key, Value, Hash, Equal>::operator*()
{
  return *list_iter_;
}

template <class Key, class Value, class Hash, class Equal>
void lavrentev::HashIter<Key, Value, Hash, Equal>::next()
{
  while (bucket_ptr_ != bucket_end_ && list_iter_ == (*bucket_ptr_).end())
  {
    ++bucket_ptr_;
    if (bucket_ptr_ != bucket_end_){
      list_iter_ = (*bucket_ptr_).begin();
    }
  }
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashCIter<Key, Value, Hash, Equal>::HashCIter(
  List<Node> *ptr,
  List<Node> *end,
  LCIter<Node> lit)
    : bucket_ptr_(ptr), bucket_end_(end), list_iter_(lit)
{
  if (bucket_ptr_ != bucket_end_ && list_iter_ == (*bucket_ptr_).cend())
  {
    next();
  }
}

template <class Key, class Value, class Hash, class Equal>
bool lavrentev::HashCIter<Key, Value, Hash, Equal>::operator==(
    const HashCIter<Key, Value, Hash, Equal> &other) const
{
  return list_iter_ == other.list_iter_ && bucket_ptr_ == other.bucket_ptr_;
}

template <class Key, class Value, class Hash, class Equal>
bool lavrentev::HashCIter<Key, Value, Hash, Equal>::operator!=(
    const HashCIter<Key, Value, Hash, Equal> &other) const
{
  return !(*this == other);
}

template <class Key, class Value, class Hash, class Equal>
lavrentev::HashCIter<Key, Value, Hash, Equal> &
lavrentev::HashCIter<Key, Value, Hash, Equal>::operator++()
{
  ++list_iter_;
  next();
  return *this;
}

template <class Key, class Value, class Hash, class Equal>
const typename lavrentev::HashTable<Key, Value, Hash, Equal>::Node &lavrentev::HashCIter<Key, Value, Hash, Equal>::operator*() const
{
  return *list_iter_;
}

template <class Key, class Value, class Hash, class Equal>
void lavrentev::HashCIter<Key, Value, Hash, Equal>::next()
{
  while (bucket_ptr_ != bucket_end_ && list_iter_ == (*bucket_ptr_).cend())
  {
    ++bucket_ptr_;
    if (bucket_ptr_ != bucket_end_){
      list_iter_ = (*bucket_ptr_).cbegin();
    }
  }
}

template <class Key, class Value, class Hash, class Equal>
void lavrentev::HashTable<Key, Value, Hash, Equal>::swap(
  HashTable<Key, Value, Hash, Equal>& other) noexcept
{
  std::swap(ht_, other.ht_);
  std::swap(slots_, other.slots_);
  std::swap(size_, other.size_);
  std::swap(loadFactor_, other.loadFactor_);
  std::swap(hasher_, other.hasher_);
  std::swap(equal_, other.equal_);
}

#endif
