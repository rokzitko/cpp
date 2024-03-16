#include <iostream>
#include <string>

int main()
{
  struct qn {
    int q;
    double sz;
    char parity {1};
  };

  qn I {.q=0, .sz=0.5, .parity=1};
  qn I2 {.q=0, .sz=0.5 }; // parity is default initialized
}
