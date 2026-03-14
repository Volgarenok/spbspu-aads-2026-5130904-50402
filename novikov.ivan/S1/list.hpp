#ifndef LIST_HPP
#define LIST_HPP

namespace novikov
{
  template < class T >
  class List;

  template < class T >
  class LIter
  {
  public:
    bool operator==(const LIter< T >& other);
    bool operator!=(LIter< T > other);
    LIter< T > operator++();
    LIter< T > operator--();
    T& operator*();

  private:
    friend class List< T >;
    const typename List< T >::Node* curr = nullptr;
  };

  template < class T >
  class LCIter
  {
  public:
    bool operator==(LCIter< T > other);
    bool operator!=(LCIter< T > other);
    LCIter< T > operator++();
    LCIter< T > operator--();
    const T& operator*();

  private:
    friend class List< T >;
    const typename List< T >::Node* curr = nullptr;
  };

  template < class T >
  class List
  {
  public:
    struct Node
    {
      T value;
      Node* next = nullptr;
      Node* prev = nullptr;
    };
    List();

    List(const List< T >&);
    List(List< T >&&);

    ~List() noexcept;

    List< T > operator=(const List< T >&);
    List< T > operator=(List< T >&&);

    LIter< T > begin();
    LCIter< T > begin() const;
    LCIter< T > cbegin() const noexcept;

    LIter< T > end();
    LCIter< T > end() const;
    LCIter< T > cend() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void push_front(const T&);
    void push_front(T&&);
    void push_back(const T&);
    void push_back(T&&);

    void pop_front();
    void pop_back();

    void clear();
    bool empty() const;

  private:
    Node* head = nullptr;
    Node* tail = nullptr;
  };
}

#endif
