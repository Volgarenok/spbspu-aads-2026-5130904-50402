#include <iostream>

namespace lachugin
{
  template < class T >
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
    List(const List<T>& other);
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

  template<class T>
  List<T>::List(const List<T>& other): List()
  {
    Node<T>* curr = nullptr;

    for (auto it = other.begin(); it != other.end(); ++it)
    {
      if (curr == nullptr)
        curr = add(*it);
      else
        curr = addNext(*it, curr);
    }
  }

  template< class T >
  Node<T>* List<T>::add(const T& val)
  {
    Node<T>* n = new Node<T>{val, fake->next};
    fake->next = n;
    return n;
  }

  template< class T >
  Node< T >* List< T >::addNext(const T &val, Node<T> *h)
  {
    Node< T >* n = new Node< T >{val, h->next};
    h->next = n;
    return n;
  }

  template< class T >
  LIter<T> List<T>::begin()
  {
    return LIter< T > {fake->next};
  }

  template< class T >
  LCIter<T> List<T>::begin() const
  {
    return LCIter< T > {fake->next};
  }

  template< class T >
  LIter<T> List<T>::end()
  {
    return LIter< T > {fake};
  }

  template< class T >
  LCIter<T> List<T>::end() const
  {
    return LCIter< T > {fake};
  }

  template< class T >
  void List<T>::clear()
  {
    auto it = begin();

    while (it != end())
    {
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
  bool LIter <T >::operator==(const LIter &other)
  {
    return curr == other.curr;
  }

  template<class T>
  bool LIter<T>::operator!=(const LIter &other)
  {
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
     const Node< T > *curr;
  public:
    explicit LCIter(Node<T>* n) : curr(n) {}
    const T &operator*() const;

    LCIter &operator++();

    bool operator==(const LCIter& other) const;

    bool operator!=(const LCIter& other) const;
  };

  template < class T > const T &LCIter< T >::operator*() const
  {
    return curr->value;
  }

  template< class T >
  bool LCIter <T >::operator==(const LCIter &other) const
  {
    return curr == other.curr;
  }

  template< class T >
  bool LCIter<T>::operator!=(const LCIter &other) const
  {
    return curr != other.curr;
  }

  template< class T >
  LCIter<T> &LCIter<T>::operator++() {
    curr = curr->next;
    return *this;
  }

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

  List <std::pair< std::string, List< int >* > > getline(std::istream& in)
  {
    using pair = std::pair< std::string, List< int >* >;

    List < pair > listOfLists{};
    Node < pair >* currOfLists = nullptr;

    std::string name;
    try {
      while (in >> name)
      {
        auto* list = new List< int >{};
        Node< int >* curr = nullptr;
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

        in.clear();
        pair p{name, list};

        if (currOfLists == nullptr)
        {
          currOfLists = listOfLists.add(p);
        }
        else
        {
          currOfLists = listOfLists.addNext(p, currOfLists);
        }
      }
    } catch (const std::bad_alloc& e) {
      std::cout << e.what();
      listOfLists.clear();
      throw;
    }

    return listOfLists;
  }
}
