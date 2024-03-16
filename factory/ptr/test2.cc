#include <iostream>
#include <string>
#include "factory.h"

class Base {};

struct Derived0 : public Base {
  Derived0() = default;
  static Base* Produce(std::string value)
  {
    if(value == "rok")
      return new Derived0();
    return nullptr;
  }
};

struct Derived1 : public Base {
  Derived1() = default;
  static Base* Produce(std::string value)
  {
    if(value == "zitko")
      return new Derived1();
    return nullptr;
  }
};

int main()
{
  using Factory = AbstractFactory<Base, MakeTypeList<Derived0, Derived1>::Type>;
  auto b1 = Factory::Produce("rok");
  auto b0 = Factory::Produce("zitko");
  auto b2 = Factory::Produce("danaja");
  std::cout << b0 << ", " << b1 << ", " << b2 << std::endl;
}

