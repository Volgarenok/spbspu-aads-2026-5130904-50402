#include <iostream>
#include "list.hpp"

int main()
{
	khalikov::List< size_t > list;
	list.pushBack(1);
	list.pushBack(2);
	list.pushBack(3);
	list.pushBack(33);
	list.pushBack(53);
	using pair_t = std::pair< const char *, khalikov::List< size_t > >;
	pair_t sequences[]  = {
		{"first", list}
	};
	std::cout << sequences[0].first << '\t';
	sequences[0].second.print();
	std::cout << '\n';
}
