#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <format>
#include <print>
#include <ranges>

template <typename T, std::predicate<const T&> Pred>
auto FilterRangesFilter(const std::vector<T>& vec, Pred p) {
   std::vector<T> out;
   for (const auto& elem : vec 
	| std::views::filter(p))
     out.push_back(elem);
   return out;
}

int main()
{
   std::println("Hello world!");
   std::vector<int> v {1,2,3};
   auto out = FilterRangesFilter(v, [](int i) 
				 {
				    return i%2;
				 }
				);
   std::println("out={}", out);
   std::println("out={::}", out);
   return 0;
}
