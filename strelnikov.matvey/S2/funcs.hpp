#include <iostream>
#include <string>
#include "datastructs.hpp"

namespace strelnikov
{
  int getPriority(const std::string &);
  bool checkPriority(const std::string &, const std::string &);
	Queue< std::string > convertToPostfix(Queue< std::string >);
	Queue< Queue < std::string > > input(std::istream &);
	long long calcOps(const std::string &, long long, long long);
	long long calc(Queue< std::string >);
	long long notFunc(long long);
	bool isOp(const std::string &);
	void print(Queue< long long > &);
	bool isNumber(const std::string &);
}