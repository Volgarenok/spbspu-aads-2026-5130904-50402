#include <iostream>
#include "list.hpp"

using pair_t = std::pair< std::string, khalikov::List< size_t > >;

void print(pair_t * res, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		std::cout << res[i].first << (i == size - 1 ? "" : " ");
	}
	std::cout << '\n';
	khalikov::List< size_t > sums;
	while (true)
	{
		size_t sum = 0;
		bool hasNumbers = false;
		bool isFirst = true;
		for (size_t i = 0; i < size; ++i)
		{
			if (!res[i].second.isEmpty())
			{
				size_t val = *(res[i].second.begin());
				std::cout << (isFirst ? "" : " ") << val;
				sum += val;
				res[i].second.popFront();
				hasNumbers = true;
				isFirst = false;
			}
		}
		if (!hasNumbers)
		{
			break;
		}
		std::cout << '\n';
		sums.pushBack(sum);
	}
	if (!sums.isEmpty())
	{
		auto it = sums.cbegin();
		for (size_t i = 0; i < sums.size(); ++i)
		{
			std::cout << (i == 0 ? "" : " ") << *it;
			++it;
		}
		std::cout << '\n';
	}
}

std::pair< std::string, khalikov::List< size_t > > enterLine(std::istream & in)
{
	std::string name;
	in >> name;
	if (in.fail())
	{
		return {};
	}
	khalikov::List< size_t > list;
	char c = 0;
	bool is_skipws = in.flags() & std::ios_base::skipws;
	if (is_skipws)
	{
		in >> std::noskipws;
	}
	while (in >> c && c != '\n')
	{
		if (std::isdigit(c))
		{
			in.putback(c);
			size_t num = 0;
			in >> std::skipws >> num >> std::noskipws;
			list.pushBack(num);
		}
		else if (!std::isspace(c))
		{
			in.setstate(std::ios::failbit);
			break;
		}
	}
	if (is_skipws)
	{
		in >> std::skipws;
	}
	return {name, list};
}

template< class T >
T * extend(T * arr, size_t & size, size_t & cap)
{
	if (size < cap)
	{
		return arr;
	}
	T * res = nullptr;
	size_t newCap = cap + 5;
	try
	{
		res = new T[newCap];
		for (size_t i = 0; i < size; i++)
		{
			res[i] = arr[i];
		}
	}
	catch (const std::bad_alloc &)
	{
		delete[] res;
		throw;
	}
	delete[] arr;
	cap = newCap;
	return res;
}

int main()
{
	using pair_t = std::pair< std::string, khalikov::List< size_t > >;
	pair_t * res = nullptr;
	size_t size = 0;
	size_t cap = 5;
	try
	{
		res = new pair_t [cap];
		while (std::cin)
		{
			pair_t temp = enterLine(std::cin);
			if (std::cin.fail())
			{
				if (std::cin.eof())
				{
					break;
				}
				std::cerr << "bad input" << '\n';
				delete[] res;
				return 1;
			}
			res = extend(res, size, cap);
			res[size++] = temp;
		}
	}
	catch (...)
	{
		std::cerr << "memory_error" << '\n';
		delete[] res;
		return 1;
	}
	if (size == 0)
	{
		delete[] res;
		return 0;
	}
	print(res, size);
	delete[] res;
}
