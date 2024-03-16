#include <iostream>
#include <utility>

// Hruske in jabolka

int main()
{
  int x = -3;
  unsigned int y = 7;

  std::cout << std::boolalpha;
  std::cout << (x < y) << std::endl;
  std::cout << std::cmp_less(x, y) << std::endl;
}
