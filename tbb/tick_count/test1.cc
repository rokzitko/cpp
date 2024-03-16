#include <iostream>
#include <tbb/tick_count.h>

using tc = tbb::tick_count;

int main()
{
  auto t0 = tc::now();
  long j = 0;
  for (size_t i = 0 ; i < 1'000'000'000; i++) { j = j+1; }
  auto t1 = tc::now();
  std::cout << "j=" << j << std::endl;
  std::cout << "run time: " << (t1-t0).seconds() << std::endl;
}
