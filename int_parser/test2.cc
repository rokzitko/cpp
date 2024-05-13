#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include <iomanip>

#include "../parser.h"

using namespace std::string_literals;

// Parser supporting decimal, hex, octal and binary input.

uint32_t parse_uint32_t(std::string s)
{
   if (s.substr(0, 2) == "0b"s) {
      return strtoul(s.substr(2).c_str(), 0, 2);
   } else {
      std::stringstream ss(s);
      ss >> std::setbase(0);
      uint32_t i;
      ss >> i;
      return i;
   }
}

std::string dump(uint32_t a, std::string sep = "=")
{
   std::stringstream s;
   s << std::dec << a << sep << "0x" << std::hex << a << sep << "0b" << std::bitset<32>(a);
   return s.str();
}

uint64_t parse_uint64_t(std::string s)
{
   if (s.substr(0, 2) == "0b"s) {
      return strtoull(s.substr(2).c_str(), 0, 2);
   } else {
      std::stringstream ss(s);
      ss >> std::setbase(0);
      uint64_t i;
      ss >> i;
      return i;
   }
}

std::string dump(uint64_t a, std::string sep = "=")
{
   std::stringstream s;
   s << std::dec << a << sep << "0x" << std::hex << a << sep << "0b" << std::bitset<64>(a);
   return s.str();
}

int main(int argc, char *argv[])
{
   InputParser input(argc, argv);

   std::string s = "0x123";
   if (input.exists("-n"))
     s = input.get("-n");
   auto a = parse_uint32_t(s);
   std::cout << a << " -> " << dump(a) << std::endl;
}
