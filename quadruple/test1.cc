#include <iostream>
#include <quadmath.h>

typedef __float128 quad;

int main()
{
   quad x = 1.1;
   quad y = 1e-18;
   std::cout << "x=" << double(x) << " y=" << double(y) << std::endl;
}
