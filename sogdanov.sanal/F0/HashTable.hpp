#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <string>
#include <functional>
#include <stdexcept>
#include "../common/Vec.hpp"

namespace sogdanov {

  template< class Key, class Value >
  class HashTable {
  public:
    enum class State {
      EMPTY,
      OCCUPIED,
      DELETED
    };

    struct Node {
      Key key;
      Value value;
      State state;

      Node();
    };

    struct Entry {
      Key key;
      Value value;
    };

    HashTable();
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    ~HashTable();

    void insert(const Key& key, const Value& value);
    Value* find(const Key& key);
    const Value* find(const Key& key) const;
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    void remove(const Key& key);
    std::size_t getSize() const;

    sogdanov::Vector< Key > getKeys() const;
    sogdanov::Vector< Entry > getEntries() const;
    void clear();

  private:
    Node* table_;
    std::size_t capacity_;
    std::size_t size_;

    std::size_t hash1(const Key& key) const;
    std::size_t hash2(const Key& key) const;
    void rehash();
  };

  template< class Key, class Value >
  HashTable< Key, Value >::Node::Node() :
    state(State::EMPTY)
  {
  }

  template< class Key, class Value >
  HashTable< Key, Value >::HashTable() :
    table_(new Node[16]),
    capacity_(16),
    size_(0)
  {
  }

  template< class Key, class Value >
  HashTable< Key, Value >::HashTable(const HashTable& other) :
    table_(new Node[other.capacity_]),
    capacity_(other.capacity_),
    size_(other.size_)
  {
    for (std::size_t i = 0; i < capacity_; ++i) {
      table_[i].state = other.table_[i].state;
      if (table_[i].state == State::OCCUPIED) {
        table_[i].key = other.table_[i].key;
        table_[i].value = other.table_[i].value;
      }
    }
  }

  template< class Key, class Value >
  HashTable< Key, Value >& HashTable< Key, Value >::operator=(const HashTable& other)
  {
    if (this != &other) {
      Node* new_table = new Node[other.capacity_];
      for (std::size_t i = 0; i < other.capacity_; ++i) {
        new_table[i].state = other.table_[i].state;
        if (new_table[i].state == State::OCCUPIED) {
          new_table[i].key = other.table_[i].key;
          new_table[i].value = other.table_[i].value;
        }
      }
      delete[] table_;
      table_ = new_table;
      capacity_ = other.capacity_;
      size_ = other.size_;
    }
    return *this;
  }

  template< class Key, class Value >
  HashTable< Key, Value >::~HashTable()
  {
    delete[] table_;
  }

  template< class Key, class Value >
  void HashTable< Key, Value >::insert(const Key& key, const Value& value)
  {
    if (size_ >= capacity_ / 2) {
      rehash();
    }
    std::size_t h1 = hash1(key);
    std::size_t h2 = hash2(key);
    std::size_t index = h1 % capacity_;

    while (table_[index].state == State::OCCUPIED) {
      if (table_[index].key == key) {
        table_[index].value = value;
        return;
      }
      index = (index + h2) % capacity_;
    }

    table_[index].key = key;
    table_[index].value = value;
    table_[index].state = State::OCCUPIED;
    ++size_;
  }

  template< class Key, class Value >
  Value* HashTable< Key, Value >::find(const Key& key)
  {
    std::size_t h1 = hash1(key);
    std::size_t h2 = hash2(key);
    std::size_t index = h1 % capacity_;
    std::size_t start_index = index;

    while (table_[index].state != State::EMPTY) {
      if (table_[index].state == State::OCCUPIED && table_[index].key == key) {
        return &(table_[index].value);
      }
      index = (index + h2) % capacity_;
      if (index == start_index) {
        break;
      }
    }
    return nullptr;
  }

  template< class Key, class Value >
  const Value* HashTable< Key, Value >::find(const Key& key) const
  {
    return const_cast< HashTable* >(this)->find(key);
  }

  template< class Key, class Value >
  Value& HashTable< Key, Value >::at(const Key& key)
  {
    Value* ptr = find(key);
    if (ptr == nullptr) {
      throw std::out_of_range("Key not found");
    }
    return *ptr;
  }

  template< class Key, class Value >
  const Value& HashTable< Key, Value >::at(const Key& key) const
  {
    const Value* ptr = find(key);
    if (ptr == nullptr) {
      throw std::out_of_range("Key not found");
    }
    return *ptr;
  }

  template< class Key, class Value >
  void HashTable< Key, Value >::remove(const Key& key)
  {
    std::size_t h1 = hash1(key);
    std::size_t h2 = hash2(key);
    std::size_t index = h1 % capacity_;
    std::size_t start_index = index;

    while (table_[index].state != State::EMPTY) {
      if (table_[index].state == State::OCCUPIED && table_[index].key == key) {
        table_[index].state = State::DELETED;
        --size_;
        return;
      }
      index = (index + h2) % capacity_;
      if (index == start_index) {
        break;
      }
    }
  }

  template< class Key, class Value >
  std::size_t HashTable< Key, Value >::getSize() const
  {
    return size_;
  }

  template< class Key, class Value >
  sogdanov::Vector< Key > HashTable< Key, Value >::getKeys() const
  {
    sogdanov::Vector< Key > keys;
    for (std::size_t i = 0; i < capacity_; ++i) {
      if (table_[i].state == State::OCCUPIED) {
        keys.pushBack(table_[i].key);
      }
    }
    return keys;
  }

  template< class Key, class Value >
  sogdanov::Vector< typename HashTable< Key, Value >::Entry > HashTable< Key, Value >::getEntries() const
  {
    sogdanov::Vector< Entry > entries;
    for (std::size_t i = 0; i < capacity_; ++i) {
      if (table_[i].state == State::OCCUPIED) {
        Entry e;
        e.key = table_[i].key;
        e.value = table_[i].value;
        entries.pushBack(e);
      }
    }
    return entries;
  }

  template< class Key, class Value >
  void HashTable< Key, Value >::clear()
  {
    delete[] table_;
    capacity_ = 16;
    size_ = 0;
    table_ = new Node[capacity_];
  }

  template< class Key, class Value >
  std::size_t HashTable< Key, Value >::hash1(const Key& key) const
  {
    return std::hash< Key >{}(key);
  }

  template< class Key, class Value >
  std::size_t HashTable< Key, Value >::hash2(const Key& key) const
  {
    std::size_t hash = std::hash< Key >{}(key);
    return (hash / capacity_) | 1;
  }

  template< class Key, class Value >
  void HashTable< Key, Value >::rehash()
  {
    std::size_t old_capacity = capacity_;
    Node* old_table = table_;

    capacity_ *= 2;
    table_ = new Node[capacity_];
    size_ = 0;

    for (std::size_t i = 0; i < old_capacity; ++i) {
      if (old_table[i].state == State::OCCUPIED) {
        insert(old_table[i].key, old_table[i].value);
      }
    }
    delete[] old_table;
  }

}

#endif
