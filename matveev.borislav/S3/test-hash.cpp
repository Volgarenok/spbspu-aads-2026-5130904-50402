#include <boost/test/unit_test.hpp>

#include "hash-table.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>

namespace
{
struct IntHash
{
  size_t operator()(int value) const
  {
    return static_cast< size_t >(value);
  }
};

struct IntEqual
{
  bool operator()(int lhs, int rhs) const
  {
    return lhs == rhs;
  }
};

struct SameHash
{
  size_t operator()(int) const
  {
    return 0;
  }
};

size_t addOne(size_t value)
{
  return value + 1;
}
}

BOOST_AUTO_TEST_CASE(add_and_find_value)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(2, "two");

  BOOST_TEST(table.has(1));
  BOOST_TEST(table.has(2));
  BOOST_TEST(!table.has(3));

  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");

  BOOST_TEST(table.size() == 2);
}

BOOST_AUTO_TEST_CASE(duplicate_key_throws)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");

  BOOST_CHECK_THROW(table.add(1, "another"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(drop_removes_value)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(2, "two");

  BOOST_TEST(table.drop(1) == "one");

  BOOST_TEST(!table.has(1));
  BOOST_TEST(table.has(2));

  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(clear_removes_all_values)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(2, "two");

  table.clear();

  BOOST_TEST(table.size() == 0);

  BOOST_TEST(!table.has(1));
  BOOST_TEST(!table.has(2));
}

BOOST_AUTO_TEST_CASE(uses_overflow_bucket)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(1, 2);

  table.maxLoadFactor(100.0);
  table.maxSpareBucketSize(100);
  table.maxAverageBucketSize(100.0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");
  BOOST_TEST(table.at(3) == "three");

  BOOST_TEST(table.size() == 3);
}

BOOST_AUTO_TEST_CASE(overflow_bucket_limit_throws)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(1, 2);

  table.maxLoadFactor(100.0);
  table.maxSpareBucketSize(100);
  table.maxAverageBucketSize(100.0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  table.add(4, "four");

  BOOST_CHECK_THROW(table.add(5, "five"), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(rehash_keeps_values)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(1, 2);

  table.maxLoadFactor(100.0);
  table.maxSpareBucketSize(100);
  table.maxAverageBucketSize(100.0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  table.rehash(2, 3);

  BOOST_TEST(table.bucketCount() == 2);
  BOOST_TEST(table.bucketCapacity() == 3);

  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");
  BOOST_TEST(table.at(3) == "three");

  BOOST_TEST(table.size() == 3);
}

BOOST_AUTO_TEST_CASE(copy_constructor_copies_values)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(1, 2);

  table.maxLoadFactor(100.0);
  table.maxSpareBucketSize(100);
  table.maxAverageBucketSize(100.0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  matveev::HashTable< int, std::string, SameHash, IntEqual > copy(table);

  BOOST_TEST(copy.size() == 3);

  BOOST_TEST(copy.at(1) == "one");
  BOOST_TEST(copy.at(2) == "two");
  BOOST_TEST(copy.at(3) == "three");
}

BOOST_AUTO_TEST_CASE(assignment_operator_copies_values)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > source(1, 2);

  source.maxLoadFactor(100.0);
  source.maxSpareBucketSize(100);
  source.maxAverageBucketSize(100.0);

  source.add(1, "one");
  source.add(2, "two");
  source.add(3, "three");

  matveev::HashTable< int, std::string, SameHash, IntEqual > target(2, 2);

  target = source;

  BOOST_TEST(target.size() == 3);

  BOOST_TEST(target.at(1) == "one");
  BOOST_TEST(target.at(2) == "two");
  BOOST_TEST(target.at(3) == "three");
}

BOOST_AUTO_TEST_CASE(iterator_visits_only_occupied_items)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(1, 2);

  table.maxLoadFactor(100.0);
  table.maxSpareBucketSize(100);
  table.maxAverageBucketSize(100.0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  size_t count = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }

  BOOST_TEST(count == 3);
}

BOOST_AUTO_TEST_CASE(hash_table_statistics)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(2, 2);

  table.maxLoadFactor(100.0);
  table.maxSpareBucketSize(100);
  table.maxAverageBucketSize(100.0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.bucketCount() == 2);
  BOOST_TEST(table.bucketCapacity() == 2);
  BOOST_TEST(table.loadFactor() == 0.75);
  BOOST_TEST(table.averageBucketSize() == 1.5);
  BOOST_TEST(table.maxBucketSize() == 2);
  BOOST_TEST(table.spareBucketSize() == 1);
}

BOOST_AUTO_TEST_CASE(auto_rehash_by_load_factor)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(2, 2);

  table.maxLoadFactor(0.5);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_TEST(table.bucketCount() > 2);
  BOOST_TEST(table.has(1));
  BOOST_TEST(table.has(2));
  BOOST_TEST(table.has(3));
}

BOOST_AUTO_TEST_CASE(auto_rehash_by_spare_bucket_size)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(2, 2);

  table.maxLoadFactor(100.0);
  table.maxAverageBucketSize(100.0);
  table.maxSpareBucketSize(0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_TEST(table.bucketCapacity() > 2);
  BOOST_TEST(table.has(1));
  BOOST_TEST(table.has(2));
  BOOST_TEST(table.has(3));
}

BOOST_AUTO_TEST_CASE(custom_bucket_update_function_is_used)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(2, 2);

  table.bucketUpdateFunction(addOne);
  table.maxLoadFactor(0.5);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_TEST(table.bucketCount() == 3);
  BOOST_TEST(table.has(1));
  BOOST_TEST(table.has(2));
  BOOST_TEST(table.has(3));
}

BOOST_AUTO_TEST_CASE(custom_bucket_size_update_function_is_used)
{
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(2, 2);

  table.bucketSizeUpdateFunction(addOne);
  table.maxLoadFactor(100.0);
  table.maxAverageBucketSize(100.0);
  table.maxSpareBucketSize(0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_TEST(table.bucketCapacity() == 3);
  BOOST_TEST(table.has(1));
  BOOST_TEST(table.has(2));
  BOOST_TEST(table.has(3));
}

BOOST_AUTO_TEST_CASE(invalid_rebuild_settings_throw)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(2, 2);

  BOOST_CHECK_THROW(table.maxLoadFactor(0.0), std::invalid_argument);
  BOOST_CHECK_THROW(table.maxAverageBucketSize(0.0), std::invalid_argument);
  BOOST_CHECK_THROW(table.bucketUpdateFunction(nullptr), std::invalid_argument);
  BOOST_CHECK_THROW(table.bucketSizeUpdateFunction(nullptr), std::invalid_argument);
}
