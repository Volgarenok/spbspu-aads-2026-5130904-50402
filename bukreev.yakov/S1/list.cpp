#include "list.hpp"

namespace bukreev
{
  template< class T >
  List< T >::List()
  {
    m_fake.next = nullptr;
  }
}
