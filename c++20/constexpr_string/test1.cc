#include <iostream>
#include <version>
#include <cassert>

// CppCon20: C++20 STL Features, Stephan T. Lavavej

#ifdef __cpp_lib_constexpr_string
#include <string>
#else
#error "no constexpr string"
#endif

constexpr std::string test { "test string" };

int main()
{
}
