#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <ranges>

int main()
{
  std::println("Hello world!");
  std::vector<std::string> v{"a", "b" ,"c"};
  std::ranges::shift_left(v, 1);
}
