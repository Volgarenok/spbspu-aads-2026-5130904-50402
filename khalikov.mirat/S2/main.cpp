#include <iostream>
#include <string>
#include <fstream>

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
