#include <iostream>
#include <version>
#include <cassert>
#include <thread>

int main()
{
  std::jthread th1 { [] { /* do work */ } };

  // no join required!
}
