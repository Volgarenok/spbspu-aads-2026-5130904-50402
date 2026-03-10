#include <iostream>

namespace lachugin
{
  template <class T >
  struct Node
  {
    T value;
    Node* next;
  };

  template< class T >
  class List
  {
    Node<T>* fake;
  public:
    List();
    Node< T >* addNext(const T& val, Node< T >* h);
    Node< T >* add(const T& val);

  };

  template< class T >
  List<T>::List()
  {
    fake = new Node< T >;
    fake->next = fake;
  }

  template<class T>
  Node<T>* List<T>::add(const T& val)
  {
    Node<T>* n = new Node<T>{val, fake->next};
    fake->next = n;
    return n;
  }

  template< class T >
  Node< T >* List< T >::addNext(const T &val, Node<T> *h) {
    Node< T >* n = new Node< T >{val, h->next};
    h->next = n;
    return n;
  }

  template < class T >
  class LIter
  {
    friend class List< T >;
    Node< T > *curr;
  public:
    explicit LIter(Node<T>* n) : curr(n) {}
  };

  template < class T >
  class LCIter
  {
    friend class List< T >;
    const Node< const T > *curr;
  };


  std::pair< std::string, List< int >* >* getline(std::istream &in, size_t& size)
  {
    size = 0;
    std::string name;
    List < int >* h = nullptr;

    while (in >> name) {

    }
  }

}
