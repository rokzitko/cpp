#include <iostream>
#include <string>
#include <print>
#include <mdspan>

int main()
{
  std::println("Hello world!");
  int data[4] = { 0,1,2,3 };
  auto sp { std::mdspan(data, 2, 2) };
}
