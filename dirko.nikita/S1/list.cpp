#include "list.hpp"

template < class T >
dirko::List< T >::List():
  fake_(static_cast< dirko::Node< T > * >(::operator new(sizeof(dirko::Node< T >)))),
  size_(0)
{}
