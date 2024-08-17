#include <iostream>
#include <string>
#include <print>
#include <set>
#include <vector>
#include <ranges>

int main()
{
  std::println("Hello world!");
  std::vector v { 3, 1, 2 };
  auto set { std::ranges::to<std::set>(v) };
}
