#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

// https://en.cppreference.com/w/cpp/ranges/iota_view

void print(auto const &seq) {
  for (auto const &elem: seq) std::cout << elem << ' ';
  std::cout << std::endl;
}

void print_view(auto &result)
{
// https://timur.audio/how-to-make-a-container-from-a-c20-range
   std::vector<std::ranges::range_value_t<decltype(result)>> v;
   std::ranges::copy(result, std::back_inserter(v));
   print(v);
}

namespace ranges = std::ranges;
namespace views = ranges::views;

int main()
{
   auto r1 { views::iota(10) | views::take(20) };
   print_view(r1);
   auto r2 { ranges::iota_view(10,30) };
   print_view(r2);
   auto r3 { views::iota(10,30) };
   print_view(r3);
}
