#include <iostream>
#include <string>
#include <print>
#include <bit>

int main()
{
  std::cout << "Hello world!" << std::endl;
  std::uint32_t a { 0x12345678 };
  std::println("{:x}", a);
  std::uint32_t b { std::byteswap(a) };
  std::println("{:x}", b);
}
