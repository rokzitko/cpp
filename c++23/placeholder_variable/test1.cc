#include <iostream>
#include <string>

int foo() { return 1; }

int main()
{
  std::cout << "Hello world!" << std::endl;
  auto _ = 42;
  auto _ = foo();
}

