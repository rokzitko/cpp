#include <iostream>
#include <string>
#include <memory>
#include "factory.h"

class Base {};

struct Derived0 : public Base {
  Derived0() = default;
  static std::shared_ptr<Base> Produce(std::string value)
  {
    return value == "rok" ? std::make_shared<Derived0>() : nullptr;
  }
};

struct Derived1 : public Base {
  Derived1() = default;
  static std::shared_ptr<Base> Produce(std::string value)
  {
    return value == "zitko" ? std::make_shared<Derived1>() : nullptr;
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

