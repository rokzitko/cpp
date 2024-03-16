#include <iostream>
#include <version>
#include <cassert>
#include <thread>

int main()
{
  std::jthread th1 { [](std::stop_token token) { 
    while (!token.stop_requested()) {
      /* do work */ 
    } 
  } };

  th1.request_stop();
}
