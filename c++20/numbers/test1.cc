#include <iostream>
#include <version>
#include <cassert>
#include <numbers>

int main()
{
  using namespace std;
  cout << "e=" << numbers::e << endl;
  cout << "log2e=" << numbers::log2e << endl;
  cout << "pi=" << numbers::pi << endl;
  cout << "inv_pi=" << numbers::inv_pi << endl;
  cout << "inv_sqrtpi=" << numbers::inv_sqrtpi << endl;
  cout << "sqrt2=" << numbers::sqrt2 << endl;
}
