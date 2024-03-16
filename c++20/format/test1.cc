#include <iostream>
#include <version>
#include <cassert>

// CppCon20: C++20 String formatting (Marc Gregoire)

#ifdef __cpp_lib_format
#include <format>
#else
#error "no format"
#endif

int main()
{
  using namespace std;
  cout << format("{:=^19}", "test") << endl;
  cout << format("Read {0} bytes from {1}", 500, "file.txt") << endl;
}
