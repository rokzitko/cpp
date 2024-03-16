#include <iostream>
#include <tbb/info.h>
#include <tbb/task_group.h>
#include <fmt/format.h>

using namespace tbb;

int Fib(int n) {
  if( n<2 ) {
    return n;
  } else {
    int x, y;
    task_group g;
    g.run([&]{x=Fib(n-1);}); // spawn a task
    g.run([&]{y=Fib(n-2);}); // spawn another task
    g.wait();                // wait for both tasks to complete
    return x+y;
  }
}

int main()
{
  for (int i = 0; i <= 10; i++)
    std::cout << fmt::format(FMT_STRING("Fib({})={}"), i, Fib(i)) << std::endl; // compile time check
}
