#ifndef HASH_ITERS_HPP
#define HASH_ITERS_HPP

namespace afanasev
{
  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    ...
    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);
    ...
    using const_iterator = BSTConstIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);
    ...
    size_t height(const_iterator it);
    size_t height();
    ...
  };
}

#endif
