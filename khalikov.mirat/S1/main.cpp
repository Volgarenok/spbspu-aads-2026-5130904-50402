#include <iostream>
#include "list.hpp"


int main()
{

	khalikov::List< int > l;
	l.pushFront(4);
	l.pushFront(2);
	l.pushFront(3);
	l.pushFront(11);
	l.pushFront(5);
	l.pushFront(4);
	l.pushFront(7);
	l.pushFront(2);
	l.pushFront(2);
	l.print();
	l.uniqueSort();
	l.print();
	l.reverse();
	l.print();
}
