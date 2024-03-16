#include <iostream>
#include <string>
#include <vector>

int main()
{
  auto l = []<typename ...T>(T&& ...args) { return foo(std::forward<T>(args)...); };
}
