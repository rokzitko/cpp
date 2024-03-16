#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <execution>

using sc = std::chrono::steady_clock;
using tp = sc::time_point;
using vec = std::vector<double>;

void duration(tp start, tp end, std::string msg = "Elapsed time")
{
  auto diff = end-start;
  std::cout << msg << ' ' << std::chrono::duration<double,std::milli>(diff).count() << std::endl;
}

template<typename T>
  void test(const T &policy, const vec &data, const int repeat)
{
  for(int i = 0; i < repeat; i++) {
    vec my_data(data);
    const auto start = sc::now();
    std::sort(policy, begin(my_data), end(my_data));
    const auto end = sc::now();
    duration(start, end);
  }
  std::cout << std::endl;
}
   

int main()
{
  constexpr int seed{1234};
  constexpr int samples{5'000'000};
  constexpr int repeat{10};

  std::default_random_engine dre(seed);
  std::uniform_real_distribution<double> di(10.0,20.0);
  vec data(samples);
  for(auto &e : data)
    e = di(dre);

  std::cout << "std::execution::seq\n";
  test(std::execution::seq, data, repeat);
  std::cout << "std::execution::par\n";
  test(std::execution::par, data, repeat);  
}

