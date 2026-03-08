#ifndef BILIST_HPP
#define BILIST_HPP

namespace shirokov
{
  template < class T >
  class BiList;

  template < class T >
  class BLIter
  {
  public:
    bool operator==(const BLIter< T >& other);
    bool operator!=(BLIter< T > other);
    BLIter< T > operator++();
    BLIter< T > operator--();
    T& operator*();

  private:
    friend class BiList< T >;
    const typename BiList< T >::Node* curr = nullptr;
  };

  template < class T >
  class BLCIter
  {
  public:
    bool operator==(BLCIter< T > other);
    bool operator!=(BLCIter< T > other);
    BLCIter< T > operator++();
    BLCIter< T > operator--();
    const T& operator*();

  private:
    friend class BiList< T >;
    const typename BiList< T >::Node* curr = nullptr;
  };

  template < class T >
  class BiList
  {
  public:
    struct Node
    {
      T value;
      Node* next = nullptr;
      Node* prev = nullptr;
    };
    BiList< T >();

    BiList< T >(const BiList< T >&);
    BiList< T >(BiList< T >&&);

    ~BiList< T >() noexcept;

    BiList< T > operator=(const BiList< T >&);
    BiList< T > operator=(BiList< T >&&);

    BLIter< T > begin();
    BLCIter< T > begin() const;
    BLCIter< T > cbegin() const noexcept;

    BLIter< T > end();
    BLCIter< T > end() const;
    BLCIter< T > cend() const noexcept;

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
