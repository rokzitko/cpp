#include <iostream>

int main()
{
   std::cout << reinterpret_cast<const char*>(u8"\u2191 \u2193\n");   // ↑ ↓  (U+2191, U+2193) encoded as UTF-8
}
