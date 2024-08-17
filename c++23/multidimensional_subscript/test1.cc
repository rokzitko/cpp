#include <iostream>
#include <string>

class Cell {
 public:
   int & operator[](size_t x, size_t y, size_t z) noexcept {
     return val;
   }
 private:
  int val {};
};

int main()
{
  std::cout << "Hello world!" << std::endl;
  Cell c;
  std::cout << c[1,2,3] << std::endl;
  c[1,2,3] = 2;
  std::cout << c[1,2,3] << std::endl;
}

