#include <iostream>
#include <string>


int main()
{
  std::cout << "Hello world!" << std::endl;
  auto a = [] [[nodiscard]] () [[deprecated]] { return 42; };
  std::cout << a() << std::endl;
  a();
}

