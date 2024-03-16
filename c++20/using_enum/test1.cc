#include <iostream>
#include <string>
#include <vector>

enum class rgb { red, green, blue };

std::string f(rgb x)
{
  switch (x) {
  using enum rgb; // not yet?
  case red: return "red";
  case green: return "green";
  case blue: return "blue";
  }
}

int main()
{
  std::cout << f(rgb::red) << std::endl;
}
