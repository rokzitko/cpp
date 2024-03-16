#include <iostream>
#include <string>

template<typename T>
  concept Geo = requires(T x) {
    { x.draw() } -> void;
  };

template<Geo T>
  void mydraw(T const & obj) {
    obj.draw();
  }

class Circle {
 public:
   void draw() const {
     std::cout << "hello!" << std::endl;
   }
};

int main()
{
  Circle c;

  mydraw(c);
}
