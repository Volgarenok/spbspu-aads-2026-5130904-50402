#include "list.hpp"

namespace muh = muhamadiarov;
template <class T>
muh::List< T >::List():
  head_(nullptr),
  count_(0)
{}
