#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <syncstream>

using namespace std::chrono_literals;

// https://en.cppreference.com/w/cpp/io/basic_osyncstream

void f(int i)
{
//  std::osyncstream{std::cout} << "This is thread " << i << std::endl;
}

void g(int i)
{
  std::cout << "This is thread " << i << std::endl;
}


int main()
{
  std::vector<std::thread> vf, vg;
  for (int i = 1; i <= 10; i++) 
    vf.emplace_back(std::thread(f,i));
  for (int i = 1; i <= 10; i++)
    vg.emplace_back(std::thread(g,i));
  for (auto &t: vf) t.join();
  for (auto &t: vg) t.join();
  std::this_thread::sleep_for(1s);
}
