#include <iostream>
#include <string>

int main()
{
  std::cout << "Hello world!" << std::endl;
  auto a { U'\N{LATIN CAPITAL Z}' };
  std::cout << a << std::endl;
}
