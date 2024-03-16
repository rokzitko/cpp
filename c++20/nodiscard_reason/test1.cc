#include <iostream>

[[nodiscard("Ignoring the return value will result in memory leaks.")]]
auto allocate()
{
  return new int;
}

int main()
{
  allocate();
}
