#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <format>
#include <print>
// #include <fmt/ranges.h>

int main()
{
  std::println("Hello world!");
//  std::vector<std::pair<int, int>> v { {1,2}, {3,4} };
  std::vector<int> v {1,2,3};
  std::println("v={}", v);
  std::println("v={::}", v);
}
