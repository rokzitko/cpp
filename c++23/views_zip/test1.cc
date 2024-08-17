#include <iostream>
#include <string>
#include <print>
#include <vector>
#include <ranges>

int main()
{
  std::println("Hello world!");
  std::vector v1 { 1,2 };
  std::vector v2 { 'a', 'b' };
  auto r { std::views::zip(v1, v2) };
}
