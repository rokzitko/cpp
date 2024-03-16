#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

template<typename T>
  concept Incrementable = requires(T t) { t++; ++t; };

template<typename T>
  concept Decrementable = requires(T t) { t--; --t; };

template<typename T> requires Incrementable<T> && Decrementable<T>
void Foo(T &t) {
  t++;
  t--;
}

template<typename T>
  concept C = Incrementable<T> && Decrementable<T>;

void Bar(C auto &t) {
  t++;
  t--;
}

int main()
{
  int a = 1;
  int b = 1;
  Foo(a);
  Bar(b);
  std::cout << (a == b) << std::endl;
}
