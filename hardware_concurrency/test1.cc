#include <iostream>
#include <string>
#include <thread>

int main()
{
  std::cout << std::thread::hardware_concurrency() << std::endl;
}

