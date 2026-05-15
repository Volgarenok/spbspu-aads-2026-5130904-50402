#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "stack.hpp"
#include "queue.hpp"

namespace novikov
{
    Stack< std::string > input(std::istream& in);
    std::string infixToPostfix(const std::string& infix);
    Queue< std::string > split(const std::string& line);
    long long eval(std::string line);
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
        long long res;
        try
        {
            res = novikov::eval(expressions.top());
        }
        catch (const std::invalid_argument&)
        {
            std::cerr << "Invalid arguments\n";
            return 1;
        }
        catch (const std::out_of_range&)
        {
            std::cerr << "Out of range\n";
            return 2;
        }
        std::cout << res;
        expressions.pop();
        if (!expressions.empty())
        {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
}

novikov::Stack< std::string > novikov::input(std::istream& in)
{
    Stack< std::string > res;
    std::string exp;
    while (std::getline(in, exp))
    {
        res.push(exp);
    }
    return res;
}

long long novikov::eval(std::string line)
{
    std::string postfix = infixToPostfix(line);
    Queue< std::string > exp = split(line);
    Stack< long long > results;
    results.push(0);
    long long operand1 = 0, operand2 = 0;
    int status = 0;

    while (!exp.empty())
    {
        std::string val = exp.front();
        if (val == "+")
        {
            if (status == 2)
            {
                results.push(operand1 + operand2);
            }
            else if (status == 1)
            {
                results.top() += operand1;
            }
            else if (status == 0)
            {
                long long op1 = results.top();
                results.pop();
                long long op2 = results.top();
                results.pop();
                results.top() = op1 + op2;
            }
            status = 0;
        }
        else if (val == "-")
        {
            if (status == 2)
            {
                results.push(operand1 - operand2);
            }
            else if (status == 1)
            {
                results.top() -= operand1;
            }
            else if (status == 0)
            {
                long long op1 = results.top();
                results.pop();
                long long op2 = results.top();
                results.pop();
                results.top() = op1 - op2;
            }
            status = 0;
        }
        else if (val == "*")
        {
            if (status == 2)
            {
                results.push(operand1 * operand2);
            }
            else if (status == 1)
            {
                results.top() *= operand1;
            }
            else if (status == 0)
            {
                long long op1 = results.top();
                results.pop();
                long long op2 = results.top();
                results.pop();
                results.top() = op1 * op2;
            }
            status = 0;
        }
        else if (val == "/")
        {
            if (status == 2)
            {
                results.push(operand1 / operand2);
            }
            else if (status == 1)
            {
                results.top() /= operand1;
            }
            else if (status == 0)
            {
                long long op1 = results.top();
                results.pop();
                long long op2 = results.top();
                results.pop();
                results.top() = op1 / op2;
            }
            status = 0;
        }
        else if (val == "%")
        {
            if (status == 2)
            {
                results.push(operand1 % operand2);
            }
            else if (status == 1)
            {
                results.top() %= operand1;
            }
            else if (status == 0)
            {
                long long op1 = results.top();
                results.pop();
                long long op2 = results.top();
                results.pop();
                results.top() = op1 % op2;
            }
            status = 0;
        }
        else if (val == "|")
        {
            if (status == 2)
            {
                results.push(operand1 | operand2);
            }
            else if (status == 1)
            {
                results.top() |= operand1;
            }
            else if (status == 0)
            {
                long long op1 = results.top();
                results.pop();
                long long op2 = results.top();
                results.pop();
                results.top() = op1 | op2;
            }
            status = 0;
        }
        else
        {
            if (status == 0)
            {
                operand1 = std::stoll(val);
                status = 1;
            }
            else if (status == 1)
            {
                operand2 = std::stoll(val);
                status = 2;
            }
        }
        exp.pop();
    }
    return results.top();
}