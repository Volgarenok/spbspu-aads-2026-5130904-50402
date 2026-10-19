#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include "manager.hpp"

namespace permyakov
{
  void bin(std::istream & in, permyakov::Manager & man);
  void sum(std::istream & in, permyakov::Manager & man);
  void multi(std::istream & in, permyakov::Manager & man);
  void make(std::istream & in, permyakov::Manager & man);
  void construct(std::istream & in, permyakov::Manager & man);
  void copy(std::istream & in, permyakov::Manager & man);
  void reverse(std::istream & in, permyakov::Manager & man);
  void get(std::istream & in, permyakov::Manager & man);
  void bayes(std::istream & in, permyakov::Manager & man);
  void deleteS(std::istream & in, permyakov::Manager & man);
  void deleteE(std::istream & in, permyakov::Manager & man);
}

#endif