#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "stack.hpp"
#include "queue.hpp"

long long op(const long long val1, const long long val2, const std::string& operation)
{
	long long res = 0;
	if (operation == "+")
	{
		res = val1 + val2;
	}
	else if (operation == "-")
	{
		res = val1 - val2;
	}
	else if (operation == "*")
	{
		res = val1 * val2;
	}
	else if (operation == "/")
	{
		res = val1 / val2;
	}
	else if (operation == "#")
	{
		std::string value1 = std::to_string(val1);
		std::string value2 = std::to_string(val2);
		std::string result = value1 + value2;
		res = std::stoll(result);
	}
	return res;
}

long long result(khalikov::Queue <std::string>& queue)
{
	khalikov::Stack < long long > stack;
	std::string str;
	while (!queue.isEmpty())
	{
		str += queue.drop() + " ";
	}
	std::string el;
	std::stringstream line(str);
	while (line >> el)
	{
		if (std::isdigit(el[0]) || (el.size() > 1 && el[0] == '-'))
		{
			stack.push(std::stoll(el));
		}
		else
		{
			long long val2 = stack.drop();
			long long val1 = stack.drop();
			stack.push(op(val1, val2, el));
		}
	}
	return stack.top();
}

size_t priority(const std::string& operation)
{
	if (operation == "+" || operation == "-")
	{
		return 0;
	}
	else if (operation == "*" || operation == "/")
	{
		return 1;
	}
	else if (operation == "#")
	{
		return 2;
	}
	else
	{
		throw std::logic_error("Operation does not exist");
	}
}

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
				while (!stack.isEmpty() && stack.top() != "(" && priority(el) <= priority(stack.top()))
				{
					queue.push(stack.drop());
				}
				stack.push(el);
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
		transform
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
