#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <chrono>

int work()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(1'000));
  std::cout << "a" << std::endl;
  int result = 3;
  return result;
}

int work_with_result(std::shared_future<int> f)
{
  int r = f.get();
  std::cout << "got result=" << r << std::endl;
  return r;
}

int main()
{
  //std::shared_future<int> f = std::async(work);
  auto f = std::async(work).share();

  auto f1 = std::async(std::launch::async, work_with_result, f);
  auto f2 = std::async(std::launch::async, work_with_result, f);
}

