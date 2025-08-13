#include <iostream>
#include <string>

int main()
{
  unsigned char rnd[] = {
#embed "random.bin"
  };
  int nr = sizeof(rnd)/sizeof(rnd[0]);
  std::cout << "Hello world! nr=" << nr << std::endl;
}
