#include <iostream>
#include <string>
#include <vector>

int main()
{
  for (auto v = std::vector{1,2,3}; auto &e : v)
    std::cout << e << std::endl;
}
