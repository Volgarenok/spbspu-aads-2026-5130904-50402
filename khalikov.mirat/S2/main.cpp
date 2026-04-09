#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "stack.hpp"
#include "queue.hpp"

template < class T >
size_t priority(const T& val);

khalikov::Queue < std::string > transform(const std::string& str)
{
	khalikov::Queue < std::string > queue;
	khalikov::Stack < std::string > stack;
	std::stringstream line(str);
	std::string el;
	while (line >> el)
	{
		if (std::isdigit(el[0]) || (el.size() > 1 && el[0] == '-'))
		{
			queue.push(el);
		}
		else if (el == "(")
		{
			stack.push(el);
		}
		else if (el == ")")
		{
			while (!stack.isEmpty() && stack.top() != "(")
			{
				queue.push(stack.drop());
			}
			if (!stack.isEmpty())
			{
				stack.pop();
			}
		}
		else
		{
			if (stack.isEmpty() || stack.top() == "(")
			{
				stack.push(el);
			}
			else if (priority(el) > priority(stack.top()))
			{
				stack.push(el);
			}
			else
			{
				while (priority(el) > priority(stack.top()) || stack.top() != "(")
				{
					queue.push(stack.drop());
				}
			}
		}
	}
	while (!stack.isEmpty())
	{
		queue.push(stack.drop());
	}
	return queue;
}

void readExp(std::istream& in, std::string& str)
{
	while (std::getline(in, str))
	{
		if (str.empty())
		{
			continue;
		}
		std::cout << str << '\n';
	}
}

int main(int argc, char ** argv)
{
	std::string str;
	if (argc == 2)
	{
		std::ifstream inputFile(argv[1]);
		if (!inputFile.is_open())
		{
			std::cerr << "Read error" << '\n';
			return 1;
		}
		readExp(inputFile, str);
	}
	else
	{
		readExp(std::cin, str);
	}
}
