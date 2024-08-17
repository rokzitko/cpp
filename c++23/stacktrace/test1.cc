#include <iostream>
#include <string>
#include <print>
#include <stacktrace>

int main()
{
  std::println("Hello world!");
  auto trace { std::stacktrace::current() };
  std::println("{}", trace);
}
