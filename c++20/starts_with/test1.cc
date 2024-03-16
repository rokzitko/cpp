#include <iostream>
#include <string>

int main()
{
  std::string s = "Hello world";
  std::cout << std::boolalpha << s.starts_with("Helo") << " " << s.starts_with("Hello") << std::endl;
}
