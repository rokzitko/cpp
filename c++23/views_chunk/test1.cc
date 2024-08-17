#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <ranges>
//#include "fmt/ranges.h"

int main()
{
  std::println("Hello world!");
  std::vector v { 1,2,3,4,5 };
  auto r { v | std::views::chunk(2) };
//  fmt::print("{}", r);
}
