#include <iostream>
#include <string>
#include <thread>
#include <future>

int work()
{
  std::cout << "a" << std::endl;
  int result = 3;
  return result;
}

int main()
{
  auto t = std::async(work);
  
  int result = t.get();
  
  std::cout << "result=" << result << std::endl;
}

