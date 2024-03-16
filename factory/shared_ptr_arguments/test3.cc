#include <iostream>
#include <string>
#include <memory>
#include "factory.h"

class Base {};

struct Derived0 : public Base {
  Derived0(int i) { std::cout << i << std::endl; }
  static std::shared_ptr<Base> Produce(std::string value, int i)
  {
    return value == "rok" ? std::make_shared<Derived0>(i) : nullptr;
  }
};

struct Derived1 : public Base {
  Derived1(int i) { std::cout << i << std::endl; }
  static std::shared_ptr<Base> Produce(std::string value, int i)
  {
    return value == "zitko" ? std::make_shared<Derived1>(i) : nullptr;
  }
};

int main()
{
  using Factory = AbstractFactory<Base, MakeTypeList<Derived0, Derived1>::Type>;
  auto b1 = Factory::Produce("rok", 1);
  auto b0 = Factory::Produce("zitko", 2);
//  auto b2 = Factory::Produce("danaja", 1, 2, 3);
//  std::cout << b0 << ", " << b1 << ", " << b2 << std::endl;
}

