#include <iostream>
#include <string>
#include <print>
#include <generator>

std::generator<int> seq(int start, int nr) {
  for (int i { start }; i < start+nr; i++) {
    co_yield i;
  }
}

int main()
{
  std::println("Hello world!");
  auto gen { seq(10,5) };
  for (const auto &val: gen) {
    std::println("{}", val);
  }
}
