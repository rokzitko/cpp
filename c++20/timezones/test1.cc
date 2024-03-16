#include <iostream>
#include <version>
#include <cassert>
#include <chrono>

//using std;
using namespace std::chrono;

int main()
{
  auto localt { zoned_time { current_zone(), system_clock::now() } };
  std::cout << localt :: std::endl;
}
