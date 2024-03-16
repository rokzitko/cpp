#include <iostream>
#include <string>
#include <vector>
#include <functional>

template<class F, class... Args>
  auto delay_invoke(F f, Args... args) {
    return [f = std::move(f), args = std::move(args)...] {
      return std::invoke(f, args...);
    }
  }

void my_fnc(int i, int j) {}

int main()
{
  int val1 = 1;
  int val2 = 2;
  auto l = delay_invoke(my_fnc, val1, val2);
}
