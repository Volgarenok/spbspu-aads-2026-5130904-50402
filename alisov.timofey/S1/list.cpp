#include "list.hpp"

template < class T >
alisov::List< T >::List()
    : fake_(static_cast< alisov::Node< T > * >(::operator new(sizeof(alisov::Node< T >)))), size_(0)
{
}