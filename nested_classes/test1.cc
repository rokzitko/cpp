#include <iostream>
#include <string>
#include <list>

class level1 {
 private:
   class level2;
   std::list<level2*> all;

   class level2 {
    private:
      int x;

    public:
      level2(int _x) : x(_x) { 
      // https://en.cppreference.com/w/cpp/language/nested_types
      //  all.push_back(this); 
      }
      void hello() { std::cout << x << std::endl; }
   };

 public:
   level2 a{1};
   level2 b{2};
   level2 c{3};

   void dump() {
     for (const auto &i: all) i->hello();
   }
};

int main()
{
  level1 l1;
  l1.dump();
}
