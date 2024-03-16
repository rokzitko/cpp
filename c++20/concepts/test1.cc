#include <iostream>
#include <string>

template<typename T>
  concept Geo = requires(T x) {
    { x.draw() };
  };

template<Geo T>
  void mydraw(T const & obj) {
    obj.draw();
  }

void mydraw2(Geo auto obj) {
  obj.draw();
}

template <typename T>
void mydraw3(T obj) requires Geo<T>
{
  obj.draw();
}

class Circle {
 public:
   void draw() const {
     std::cout << "hello!" << std::endl;
   }
};

template <typename T>
concept intg = std::is_integral_v<T>;

int main()
{
  Circle c;

  mydraw(c);
  mydraw2(c);
  mydraw3(c);

  intg auto i = 12;

  auto l = []<Geo T>(T obj) { obj.draw(); };
  auto l2 = [](Geo auto obj) { obj.draw(); };

  l(c);
  l2(c);
}
