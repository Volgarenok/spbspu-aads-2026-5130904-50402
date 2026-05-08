#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "HashIter.hpp"
#include "HashFunc.hpp"
#include <stdexcept>
#include <utility>

namespace sogdanov {
    enum class HashState { EMPTY, OCCUPIED, TOMBSTONE };
    template <class Key, class Value>
    struct HashNode { 
        Key k; 
        Value v; 
        HashState state = HashState::EMPTY; 
        
        bool is_occupied() const;
    };
    template<class Key, class Value, class Hash = XXHashFunctor, class Equal = EqualFunctor<Key>>
    class HashTable {
    private:
        HashNode<Key, Value>* table_;
        size_t capacity_;
        size_t size_;
        size_t tombstones_;
        Hash hash_fn_;
        Equal eq_fn_;
    public:
        using Iterator = HashIter<HashNode<Key, Value>>;

        explicit HashTable(size_t cap = 0);
        HashTable(const HashTable& other);
        HashTable& operator=(HashTable other);
        ~HashTable();

        void swap(HashTable& other) noexcept;
        
        void add(Key k, Value v);
        Value drop(Key k);
        bool has(Key k) const;
        Value& get(Key k);
        
        void rehash(size_t slots);
        size_t size() const;

        Iterator begin();
        Iterator end();
    };


}

#endif
