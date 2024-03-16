#include <iostream>
#include <version>
#include <cassert>
#include <string>

template<std::string>
class C;

using A = C<"abc">;
using B = C<"def">;

int main()
{
}
