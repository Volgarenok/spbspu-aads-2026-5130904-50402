#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <utility>

namespace shirokov
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class HTIter
  {
  public:
    explicit HTIter(HashTable< Key, Value, Hash, Equal >&);
    bool operator==(const HTIter&) const noexcept;
    bool operator!=(const HTIter&) const noexcept;
    HTIter& operator++();
    HTIter& operator--();
    std::pair< Key, Value >& operator*();

  private:
    size_t pos_;
    HashTable< Key, Value, Hash, Equal >& table_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HTCIter
  {
  public:
    explicit HTCIter(const HashTable< Key, Value, Hash, Equal >&);
    bool operator==(const HTCIter&) const noexcept;
    bool operator!=(const HTCIter&) const noexcept;
    HTCIter& operator++();
    HTCIter& operator--();
    const std::pair< Key, Value >& operator*();

  private:
    size_t pos_;
    const HashTable< Key, Value, Hash, Equal >& table_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable();
    HashTable(const HashTable& rhs);
    HashTable(HashTable&& rhs);
    ~HashTable();
    HashTable& operator=(const HashTable& rhs);
    HashTable& operator=(HashTable&& rhs);
    Value& operator[](Key key) noexcept;

    HTIter< Key, Value, Hash, Equal > begin();
    HTCIter< Key, Value, Hash, Equal > begin() const;
    HTCIter< Key, Value, Hash, Equal > cbegin() const;

    HTIter< Key, Value, Hash, Equal > end();
    HTCIter< Key, Value, Hash, Equal > end() const;
    HTCIter< Key, Value, Hash, Equal > cend() const;

    void insert(Key k, Value v);
    void erase(Key k);
    bool contains(Key k) const;
    void rehash(size_t slots);
    bool empty() const;
    size_t slotsCount() const;
    size_t size() const;
    Value& at(Key key);

  private:
    struct Slot
    {
      Key key;
      Value value;
      bool isEmpty;
    };
    size_t size_;
    size_t slotsCount_;
    Slot* slots_;
    void expand();
  };
}

template < class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >::HashTable():
  size_(1),
  slotsCount_(1),
  slots_(nullptr)
{}

template < class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >::HashTable::~HashTable()
{
  delete[] slots_;
}

template < class Key, class Value, class Hash, class Equal >
size_t shirokov::HashTable< Key, Value, Hash, Equal >::size() const
{
  return size_;
}

template < class Key, class Value, class Hash, class Equal >
size_t shirokov::HashTable< Key, Value, Hash, Equal >::slotsCount() const
{
  return slotsCount_;
}

#endif
