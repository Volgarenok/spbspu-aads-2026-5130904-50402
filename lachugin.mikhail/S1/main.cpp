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
  class LIter;

  template< class T >
  class LCIter;

  template< class T >
  class List
  {
    Node< T >* fake;
  public:
    List();
    Node< T >* addNext(const T& val, Node< T >* h);
    Node< T >* add(const T& val);

    LIter< T > begin();
    LCIter< T > begin() const;
    LIter< T > end();
    LCIter< T > end() const;

    void clear();
  };

  template< class T >
  List<T>::List()
  {
    fake = new Node< T >;
    fake->next = fake;
  }

  template< class T >
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

  template< class T >
  LIter<T> List<T>::begin() {
    return LIter< T > {fake->next};
  }

  template< class T >
  LCIter<T> List<T>::begin() const {
    return LCIter< T > {fake->next};
  }

  template< class T >
  LIter<T> List<T>::end() {
    return LIter< T > {fake};
  }

  template< class T >
  LCIter<T> List<T>::end() const {
    return LCIter< T > {fake};
  }

  template< class T >
  void List<T>::clear() {
    auto it = begin();

    while (it != end()) {
      auto c = it;
      ++it;
      delete c.curr;
      fake->next = it.curr;
    }
    fake->next = fake;
  }


  template < class T >
  class LIter
  {
    friend class List< T >;
    Node< T > *curr;
  public:
    explicit LIter(Node<T>* n) : curr(n) {}

    LIter &operator++();

    bool operator==(const LIter& other);

    bool operator!=(const LIter& other);
  };

  template< class T >
  bool LIter <T >::operator==(const LIter &other) {
    return curr == other.curr;
  }

  template<class T>
  bool LIter<T>::operator!=(const LIter &other) {
    return curr != other.curr;
  }

  template<class T>
  LIter<T> &LIter<T>::operator++() {
    curr = curr->next;
    return *this;
  }

  template < class T >
  class LCIter
  {
    friend class List< T >;
    const Node< const T > *curr;
  };

  template<class T>
  void expand(std::pair<std::string, List<T>*>*& arr, size_t& cap, size_t size)
  {
    size_t newCap = cap * 2;

    auto* newArr = new std::pair<std::string, List<T>*>[newCap];

    for(size_t i = 0; i < size; ++i)
    {
      newArr[i] = arr[i];
    }

    delete[] arr;
    arr = newArr;
    cap = newCap;
  }

  std::pair<std::string, List<int>*>* getline(std::istream& in, size_t& size)
  {
    size_t cap = 5;
    size = 0;

    auto* arr = new std::pair<std::string, List<int>*>[cap];

    std::string name;

    try
    {
      while(in >> name)
      {
        List<int>* list = new List<int>();
        Node<int>* curr = nullptr;

        int value;

        while(in >> value)
        {
          if(curr == nullptr)
          {
            curr = list->add(value);
          }
          else
          {
            curr = list->addNext(value, curr);
          }
        }

        if(size == cap)
        {
          expand(arr, cap, size);
        }

        arr[size++] = {name, list};

        if(in.eof())
          break;

        in.clear();
      }
    }
    catch(...)
    {
      for(size_t i = 0; i < size; ++i)
      {
        arr[i].second->clear();
        delete arr[i].second;
      }

      delete[] arr;
      throw;
    }

    return arr;
  }

}
