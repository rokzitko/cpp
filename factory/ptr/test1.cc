#include <iostream>
#include <string>
#include "factory.h"

class Base {};

struct Derived0 : public Base {
  Derived0() = default;
  static Base* Produce(int value)
  {
    if(value == 0)
      return new Derived0();
    return nullptr;
  }
};

struct Derived1 : public Base {
  Derived1() = default;
  static Base* Produce(int value)
  {
    if(value == 1)
      return new Derived1();
    return nullptr;
  }
};

int main()
{
  using Factory = AbstractFactory<Base, MakeTypeList<Derived0, Derived1>::Type>;
  auto b1 = Factory::Produce(1);
  auto b0 = Factory::Produce(0);
  auto b2 = Factory::Produce(2);
  std::cout << b0 << ", " << b1 << ", " << b2 << std::endl;
}

