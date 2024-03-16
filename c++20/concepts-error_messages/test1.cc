#include <iostream>
#include <string>

// CppCon20: Back to basics: Templates, Andreas Fertig

template<typename T>
  concept Incrementable = requires(T t) { t++; ++t; };

void Foo(Incrementable auto t) {}

class Bar {};

int main()
{
  Bar b;
  Foo(b);
}
