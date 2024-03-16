#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

void g(int i)
{
  std::cout << "This is thread " << i << std::endl;
}


int main()
{
  std::thread t1(g, 1);
  std::thread t2(g, 2);
  t1.join();
  t2.join();
}
