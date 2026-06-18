#ifndef TABLE_HPP
#define TABLE_HPP

#include <utility>
#include <cstdlib>
#include <stdexcept>
#include <cmath>

namespace bukreev
{
  static bool isPrime(size_t n)
  {
    for (size_t i = 2; i < sqrt(n) + 1; i++)
    {
      if (n % i == 0)
      {
        return false;
      }
    }
    return true;
  }

  static size_t nextPrime(size_t n)
  {
    do
    {
      n++;
    } while (!isPrime(n));
    return n;
  }

  template< class K, class V, class H >
  class HashTable
  {
    using Pair = std::pair< K, V >;

  public:
    HashTable(size_t capacity = 17);
    void resize(size_t newCapacity);
    void put(K key, V val);
    V& get(K key) const;
    void erase(K key);
    bool exist(K key);
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    std::pair< K, V >* getPairs() const noexcept;
    bool occupied(size_t i) const noexcept;

  private:
    size_t hash1(const K& key) const;
    size_t hash2(const K& key) const;

  private:
    size_t mCapacity;
    size_t mSize;

    Pair* mPairs;
    bool* mOccupied;
    bool* mDeleted;
  };

  template< class K, class V, class H >
  HashTable< K, V, H >::HashTable(size_t capacity)
  {
    mCapacity = capacity;
    mPairs = new Pair[capacity];

    try
    {
      mOccupied = new bool[capacity];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] mPairs;
      throw;
    }

    try
    {
      mDeleted = new bool[capacity];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] mPairs;
      delete[] mOccupied;
      throw;
    }

    for (size_t i = 0; i < capacity; i++)
    {
      mOccupied[i] = false;
      mDeleted[i] = false;
    }

    mSize = 0;
  }

  template< class K, class V, class H >
  void HashTable< K, V, H >::resize(size_t newCapacity)
  {
    size_t oldCapacity = mCapacity;
    Pair* oldPairs = mPairs;

    mCapacity = newCapacity;

    try
    {
      mPairs = new Pair[mCapacity];
    }
    catch(const std::bad_alloc& e)
    {
      delete[] oldPairs;
      throw;
    }

    try
    {
      mOccupied = new bool[mCapacity];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] mPairs;
      delete[] oldPairs;
      throw;
    }

    try
    {
      mOccupied = new bool[mCapacity];
      mDeleted = new bool[mCapacity];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] mPairs;
      delete[] oldPairs;
      delete[] mOccupied;
      throw;
    }

    for (size_t i = 0; i < mCapacity; i++)
    {
      mOccupied[i] = false;
      mDeleted[i] = false;
    }

    mSize = 0;

    for (size_t i = 0; i < oldCapacity; i++)
    {
      put(oldPairs[i].first, oldPairs[i].second);
    }
  }

  template< class K, class V, class H >
  void HashTable< K, V, H >::put(K key, V val)
  {
    size_t h1 = hash1(key);
    size_t h2 = hash2(key);

    size_t i = 0;
    size_t id = h1;
    while (mOccupied[id] && i < mCapacity)
    {
      if (mPairs[id].first == key)
      {
        mPairs[id].second = val;
        return;
      }

      i++;
      id = (h1 + i * h2) % mCapacity;
    }

    if (i >= mCapacity)
    {
      resize(nextPrime(mCapacity));
      put(key, val);
      return;
    }

    mPairs[id].first = key;
    mPairs[id].second = val;
    mOccupied[id] = true;

    mSize++;
  }

  template< class K, class V, class H >
  V& HashTable< K, V, H >::get(K key) const
  {
    size_t h1 = hash1(key);
    size_t h2 = hash2(key);

    size_t i = 0;
    size_t id = h1;
    while ((mOccupied[id] || mDeleted[id]) && i < mCapacity)
    {
      if (mPairs[id].first == key && mOccupied[id])
      {
        return mPairs[id].second;
      }

      i++;
      id = (h1 + i * h2) % mCapacity;
    }

    throw std::out_of_range("Bad key");
  }

  template< class K, class V, class H >
  void HashTable< K, V, H >::erase(K key)
  {
    size_t h1 = hash1(key);
    size_t h2 = hash2(key);

    size_t i = 0;
    size_t id = h1;
    while ((mOccupied[id] || mDeleted[id]) && i < mCapacity)
    {
      if (mPairs[id].first == key && mOccupied[id])
      {
        mOccupied[id] = false;
        mDeleted[id] = true;
        mSize--;
        return;
      }

      i++;
      id = (h1 + i * h2) % mCapacity;
    }
  }

  template< class K, class V, class H >
  bool HashTable< K, V, H >::exist(K key)
  {
    size_t h1 = hash1(key);
    size_t h2 = hash2(key);

    size_t i = 0;
    size_t id = h1;
    while ((mOccupied[id] || mDeleted[id]) && i < mCapacity)
    {
      if (mPairs[id].first == key && mOccupied[id])
      {
        return true;
      }

      i++;
      id = (h1 + i * h2) % mCapacity;
    }

    return false;
  }

  template< class K, class V, class H >
  size_t HashTable< K, V, H >::hash1(const K& key) const
  {
    H h;
    return h(key) % mCapacity;
  }

  template< class K, class V, class H >
  size_t HashTable< K, V, H >::hash2(const K& key) const
  {
    size_t h = hash1(key);
    return (h % (mCapacity - 1)) + 1;
  }

  template< class K, class V, class H >
  size_t HashTable< K, V, H >::size() const noexcept
  {
    return mSize;
  }

  template< class K, class V, class H >
  size_t HashTable< K, V, H >::capacity() const noexcept
  {
    return mCapacity;
  }

  template< class K, class V, class H >
  std::pair< K, V >* HashTable< K, V, H >::getPairs() const noexcept
  {
    return mPairs;
  }

  template< class K, class V, class H >
  bool HashTable< K, V, H >::occupied(size_t i) const noexcept
  {
    return mOccupied[i];
  }
}

#endif
