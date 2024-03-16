#include <iostream>
#include <string>
#include <vector>

int main()
{
  auto f = []<typename T>(std::vector<T> v) { std::cout << v.size() << std::endl; };
  auto g = [](auto v) { std::cout << v.size() << std::endl; };
  
  std::vector vi {1,2,3,4};
  std::vector vd {1.0, 2.0};
  
  f(vi);
  f(vd);
  g(vi);
  g(vd);
}
