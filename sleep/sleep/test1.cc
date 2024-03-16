#include <iostream>
#include <chrono>
#include <thread>

int main()
{
  std::cout << "started" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1'000)); // C++11 ' digit group separator
  std::cout << "1s" << std::endl;
  std::this_thread::sleep_for(std::chrono::microseconds(1'000'000));
  std::cout << "2s" << std::endl;
}
