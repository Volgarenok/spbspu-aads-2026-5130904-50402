#include <fstream>
#include <iostream>
#include <string>
#include "stack.hpp"

namespace novikov
{
    Stack< std::string > input(std::istream& in);
    int eval(std::string line);
}

int main(int argc, char** argv)
{
    novikov::Stack< std::string > expressions;
    if (argc == 1)
    {
        expressions = novikov::input(std::cin);
    }
    else if (argc == 2)
    {
        std::ifstream in(argv[1]);
        expressions = novikov::input(in);
    }
    else
    {
        std::cerr << "Invalid arguments\n";
        return 1;
    }

    while (!expressions.empty())
    {
        int res = novikov::eval(expressions.top());
        std::cout << res;
        expressions.pop();
        if (!expressions.empty())
        {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
}