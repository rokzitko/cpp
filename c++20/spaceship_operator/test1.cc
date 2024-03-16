#include <iostream>
#include <version>
#include <cassert>
#include <compare>
#include <vector>

class C {
 private:
  int a = 1, b = 1;

 public:
  auto operator<=>(const C&) const = default; // memberwise comparisons
};

int main()
{
  C a,b;
  std::cout << std::boolalpha << (a==b) << std::endl;

  int i = 42;

  std::strong_ordering result { i <=> 0 };

  std::cout << (result == std::strong_ordering::greater) << std::endl;
  std::cout << is_gt(result) << std::endl;

  std::vector c { 1,2,3 };
  std::vector d { 4,5,6 };

  auto r = c <=> d;
  std::cout << is_lt(r) << std::endl;
}
