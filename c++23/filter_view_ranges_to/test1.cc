#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <format>
#include <print>
#include <ranges>

template <typename Cont, std::predicate<const typename Cont::value_type&> Pred>
  auto FilterRangesFilterTo(const Cont& vec, Pred p) {
     return vec | std::views::filter(p) 
       | std::ranges::to<Cont>();
  }

int main()
{
   std::println("Hello world!");
   std::vector<int> v {1,2,3};
   auto out = FilterRangesFilterTo(v, [](int i) 
				   {
				      return i%2;
				   }
				  );
   std::println("out={}", out);
   std::println("out={::}", out);
   return 0;
}
