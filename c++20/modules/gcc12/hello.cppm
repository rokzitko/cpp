export module hello;

namespace Hello {
  auto GetHelloStr() { return "Hello world!\n"; }
  export auto GetHello() { return GetHelloStr(); }
}
