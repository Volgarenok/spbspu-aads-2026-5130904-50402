#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
namespace karpenkov{
  template<class T> class List{
    struct Node{
      T value;
      Node* next;
      Node* prev;
      Node(const T& value, Node* next = nullptr, Node* prev = nullptr):
        value(value), next(next), prev(prev) {}
    };
    public:
      List() = default;

      void push_back(const T & v){
        Node* newNode = new Node(v);
        if (head == nullptr){
          head = newNode;
          tail = newNode;
        }
        else{
          newNode->prev = tail;
          tail->next = newNode;
          tail = newNode;
        }
      }
	  void push_front(const T & v){
        Node* newNode = new Node(v);
		if (head == nullptr){
		  head = newNode;
		  tail = newNode;
		}
		else{
		  newNode->next = head;
		  head->prev = newNode;
		  head = newNode;
		}
	  }
      void pop_back(){
		if (tail == nullptr){
		  return;
		}
		Node* temp_tail = tail->prev;
	    delete tail;
		tail = temp_tail;
		if (temp){
		  temp->next = nullptr;
		}
		else{
		  head = nullptr;
		}
	  }
	  void pop_front(){
		if (head = nullptr){
		  return;
		}
	    Node* temp_head = head->next;
		delete head;
		head = temp_head;
		if (head){
		  head->prev = nullptr;
		}
		else{
		  tail = nullptr;
		}
      }
      LIter<T> begin(){
	    return LIter<T>(head);
      }
	  LIter<T> end(){
	    return LIter<T>();
	  }
    private:
      Node* head = nullptr;
      Node* tail = nullptr;
  };
  template<class T> class LIter{
    friend class List<T>;
    private:
      typename List<T>::Node* node;
    public:
      LIter(typename List<T>::Node* n = nullptr): node(n) {}
	  T& operator*(){
	    return node->value;
      }
      LIter& operator++(){
        node = node -> next;
		return *this;
      }
      bool operator==(const LIter& rhs) const{
        return node == rhs.node;
	  }
	  bool operator!=(const LIter& rhs) const{
	    return node != rhs.node;
      }
  };
}
#endif
