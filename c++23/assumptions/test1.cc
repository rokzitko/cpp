#include <iostream>
#include <string>

int f(int x) {
  [[assume(x>0)]]; // optimization opportunity
  return x/2;
}

int main()
{
  std::cout << "Hello world!" << std::endl;
  std::cout << f(10) << std::endl;
}
