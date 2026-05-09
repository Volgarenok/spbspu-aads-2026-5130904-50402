#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <list.hpp>
#include <top-it-vector.hpp>
#include "HashFunction.hpp"

namespace afanasev
{
	template< class Key, class Value, class Hash, class Equal >
	class HashTable
	{
		using type = std::pair< Key, Value >;

		public:

		explicit HashTable(size_t slots);
    ~HashTable();

		void clear() noexcept;


		void add(Key k, Value v);
		Value drop(Key k);
		bool has(Key k);
		void rehash(size_t slots);

		private:

    Vector< List< type > > data_;
    size_t capacity_;
    size_t size_;
    Hash hasher_;
		Equal comparator_;
	};
}

template <class Key, class Value, class Hash, class Equal>
Value afanasev::HashTable<Key, Value, Hash, Equal>::drop(Key k)
{
  size_t idx = hasher_(k) % capacity_;
  List<type> & bucket = data_[idx];
  LIter<type> prev = LIter<type>();
  LIter<type> it = bucket.begin();

  while (it != LIter<type>())
  {
    if (comparator_((*it).first, k))
    {
      Value val = (*it).second;
      if (prev == LIter<type>())
      {
        bucket.popFront();
      }
      else
      {
        bucket.deleteNext(prev);
      }
      --size_;
      return val;
    }
    prev = it;
    ++it;
  }
  throw std::out_of_range("Key not found");
}

template <class Key, class Value, class Hash, class Equal>
void afanasev::HashTable<Key, Value, Hash, Equal>::add(Key k, Value v)
{
  size_t idx = hasher_(k) % capacity_;
  List<type> & bucket = data_[idx];
  LIter<type> it = bucket.begin();

  while (it != LIter<type>())
	{
    if (comparator_((*it).first, k))
		{
      (*it).second = v;
      return;
    }
    ++it;
  }

  bucket.pushFront(type(k, v));
  ++size_;
}

template < class Key, class Value, class Hash, class Equal >
void afanasev::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < capacity_; ++i)
	{
    data_[i].clear();
  }

  size_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(),
  capacity_(slots),
  size_(0),
  hasher_(Hash{}),
  comparator_(Equal{})
{
  for (size_t i = 0; i < slots; ++i)
	{
    data_.pushBack(List< type >());
  }
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  clear();
}

#endif
