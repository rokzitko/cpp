#include <iostream>
#include <string>
#include <map>

int main()
{
  std::map m = { std::pair {1, "a"}, {2, "b"}, {3, "c"} };
  auto result = m.contains(2);
  std::cout << std::boolalpha << result << std::endl;
}
