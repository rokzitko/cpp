#include <iostream>
#include <tbb/tbb_stddef.h>

int main()
{
  std::cout << "compile time: " << TBB_INTERFACE_VERSION << std::endl;
  std::cout << "run time: " << tbb::TBB_runtime_interface_version() << std::endl;
}
