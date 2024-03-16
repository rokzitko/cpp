#include <iostream>
#include <version>
#include <cassert>

// CppCon20: Back to Basic: Smart Pointers (Rainer Grimm)

#ifdef __cpp_lib_atomic_shared_ptr
#include <memory>
std::atomic<std::shared_ptr> ptr;
#else
#error "no atomic shared ptr"
#endif

int main()
{
}
