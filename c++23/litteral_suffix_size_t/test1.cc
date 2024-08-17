#include <iostream>
#include <string>
#include <vector>

int main()
{
  std::cout << "Hello world!" << std::endl;
  std::vector data {11,22,33};
  int sum;
  for (auto i = 0uz, count = data.size(); i<count; ++i) { sum += data[i]; }
  std::cout << "sum=" << sum << std::endl;
}

