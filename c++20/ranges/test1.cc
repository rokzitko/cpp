#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <functional> // greater, less

// CppCon 20: C++ ranges in action, Tristan Brindle

// https://en.cppreference.com/w/cpp/algorithm/ranges/sort

void print(auto const &seq) {
  for (auto const &elem: seq) std::cout << elem << ' ';
  std::cout << std::endl;
}

namespace ranges = std::ranges;

int main()
{
  std::vector data { 33, 22, 11, 44, 55, 66, 99, 88, 77 };

  ranges::sort(data);
  print(data);
  ranges::sort(data, ranges::less());
  print(data);
  ranges::sort(data, {});
  print(data);
  ranges::sort(data, [](int a, int b) { return a<b; });
  print(data);

  ranges::sort(data, ranges::greater());
  print(data);
  ranges::sort(data, [](int a, int b) { return a>b; });
  print(data);
}
