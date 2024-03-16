#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main()
{
  std::cout << "started" << std::endl;
  std::this_thread::sleep_for(1s); // C++11 ' digit group separator
  std::cout << "1s" << std::endl;
  std::this_thread::sleep_for(1s);
  std::cout << "2s" << std::endl;
}
