#include <iostream>
#include <iomanip>
#include <string>

#include "freqfmt.hh"

int main()
{
  std::cout << freqfmt::format_frequency(2.123123123e9, 8, '\'') << std::endl;
  std::cout << freqfmt::format_frequency(100000022, 10, '\'') << std::endl;
  std::cout << freqfmt::format_frequency(100000022.1, 10, '\'') << std::endl;
  std::cout << freqfmt::format_frequency(100000022.12, 10, '\'') << std::endl;
  std::cout << freqfmt::format_frequency(100000022.123, 10, '\'') << std::endl;
  std::cout << freqfmt::format_frequency(100000022.1, 12, '_') << std::endl;
  std::cout << freqfmt::format_frequency(100000022.12, 12, '_') << std::endl;
  std::cout << freqfmt::format_frequency(100000022.123, 12, '_') << std::endl;
}
