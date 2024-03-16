#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

void print(auto const &seq) {
  for (auto const &elem: seq) std::cout << elem << ' ';
  std::cout << std::endl;
}

namespace ranges = std::ranges;
namespace views = ranges::views;

int main()
{
   std::vector data { 33, 22, 11, 44, 55, 66, 99, 88, 77 };

   auto result = data 
     | views::filter([](const auto &v) { return v%2 == 0; })
     | views::transform([](const auto &v) { return v*2; });
//     | views::drop(1)
//     | views::reverse;
   for (const auto &x : result) std::cout << x << ' ';
   std::cout << std::endl;

// https://timur.audio/how-to-make-a-container-from-a-c20-range
   std::vector<std::ranges::range_value_t<decltype(result)>> v;
   std::ranges::copy(result, std::back_inserter(v));
   print(v);
}
