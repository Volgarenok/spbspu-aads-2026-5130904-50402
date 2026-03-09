#include "List.hpp"

std::pair<std::string, lavrentev::List<int>*>* lavrentev::getline(std::istream &in, size_t &size)
{
  size_t capacity = 10;
  size = 0;
  auto* arr = new std::pair<std::string, lavrentev::List<int>*>[capacity];
  std::string name;
  lavrentev::List<int>* head = nullptr;
  lavrentev::List<int>* curr = nullptr;

  if (!(in >> name)) {
      size = 0;
      return arr;
  }

  int value;
  try{
    while (true) {
        if (in >> value) {
            if (head == nullptr) {
                head = lavrentev::add(head, value);
                curr = head;
            } else {
                curr = lavrentev::insert(curr, value);
            }
        } else {
            if (in.eof()) {
                if (size == capacity) {
                    lavrentev::expand(&arr, capacity, size);
                }
                arr[size++] = {name, head};
                break;
            }

            in.clear();
            std::string bad;
            if (in >> bad) {
                if (size == capacity) {
                    lavrentev::expand(&arr, capacity, size);
                }
                arr[size++] = {name, head};
                name = bad;
                head = nullptr;
                curr = nullptr;
            }
        }
    }
  } catch(...) {
    lavrentev::LIter<int>(head).clear();
    for(size_t i = 0; i < size; ++i)
    {
      lavrentev::LIter<int>(arr[i].second).clear();
    }
    delete[] arr;
    throw;
  }
  if(size == 0)
  {
    delete[] arr;
    std::cerr << "Invalid input";
    throw;
  }
  return arr;
}

template <class T> T lavrentev::List<T>::retVal() const
{
  return this->val;
}

template <class T> lavrentev::List<T> *lavrentev::List<T>::retNext() const
{
  return this->next;
}

template <class T>
void lavrentev::List<T>::addNext(List<T> *aft){
  this->next = aft;
}

template <class T> void lavrentev::List<T>::addVal(int k)
{
  this->val = k;
}

template <class T>
lavrentev::List<T> *lavrentev::add(lavrentev::List<T> *h, const T &v)
{
  return new lavrentev::List<T>{v, h};
}

template <class T>
lavrentev::List<T> *lavrentev::insert(lavrentev::List<T> *h, const T &v)
{
  if(h == nullptr)
  {
    std::cerr << "Invalid pointer";
    throw;
  }
  lavrentev::List<T> *aft = add(h->retNext(), v);
  h->addNext(aft);
  return h->retNext();
}

void lavrentev::expand(std::pair<std::string, lavrentev::List<int>*> **arr, size_t &cap, size_t size)
{
  cap = cap + 5;
  auto *newArr = new std::pair<std::string, lavrentev::List<int>*>[cap];
  for(size_t i = 0; i < size; ++i)
  {
    newArr[i] = std::move((*arr)[i]);
  }
  delete[] *arr;
  *arr = newArr;
}

template <class T>
void lavrentev::LCIter<T>::printList()
{
  const List<T>* current = curr;
  while(current){
    std::cout << current->retVal() << " ";
    current = current->retNext();
  }
  std::cout << "\n";
}

template <class T>
void lavrentev::LIter<T>::clear()
{
  while(curr != nullptr)
  {
    List<T>* n = curr->retNext();
    delete curr;
    curr = n;
  }
}