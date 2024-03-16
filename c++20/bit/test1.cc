#include <iostream>
#include <bit>
#include <bitset>

int main()
{
  std::bitset<8>(0);
  std::uint8_t i = 0b01100101;
  auto b = std::rotl(i, 2);
  int count { std::popcount(b) };
  std::cout << count << std::endl;
}
