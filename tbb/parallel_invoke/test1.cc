#include <iostream>
#include <tbb/parallel_invoke.h>

int main()
{
  tbb::parallel_invoke(
                       []() { std::cout << "Hello!" << std::endl; },
                       []() { std::cout << "world" << std::endl; }
                      );
}
