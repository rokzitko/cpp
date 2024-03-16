#include <iostream>
#include <version>
#include <cassert>

// https://en.cppreference.com/w/cpp/filesystem/exists
// https://en.cppreference.com/w/cpp/filesystem

#ifdef __cpp_lib_filesystem
#include <filesystem>
#else
#error "no filesystem"
#endif

#ifdef __cpp_lib_constexpr_complex
#include <complex>
#else
#error "no constexpr complex"
#endif

constexpr std::complex z { 1, 2 };

int main()
{
  assert(std::filesystem::exists("test1.cc"));
}
