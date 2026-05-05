#ifndef HASHFUNCTION_HPP
#define HASHFUNCTION_HPP

namespace afanasev
{
	template< class Key, class Value, class Hash, class Equal >
	class HashTable
	{
		public:
		void add(Key k, Value v);
		Value drop(Key k);
		bool has(Key k);
		void rehash(size_t slots);
	};
}

#endif
