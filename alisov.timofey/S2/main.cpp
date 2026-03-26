namespace alisov
{
  template < typename T > class Queue
  {
  public:
    void push(T rhs);
    T drop();

  private:
    list< T > data_;
  };
  template < typename T > class Stack
  {
  public:
    void push(T rhs);
    T drop();

  private:
    list< T > data_;
  };
}
int main()
{
}