#ifndef HASHTITEM_HPP
#define HASHTITEM_HPP

template < class Key, class Value >
struct HashItem
{
  HashItem():
    occupied(false)
  {}
  Key key;
  Value value;
  bool occupied;
};

#endif
