// https://stackoverflow.com/questions/47981/how-to-set-clear-and-toggle-a-single-bit
// https://godbolt.org/z/Wfzh8xsjW

#include <iostream>

using Uint = unsigned long;

Uint bit_set(Uint number, Uint n) {
    return number | Uint{1} << n;
}

Uint bit_clear(Uint number, Uint n) {
    return number & ~(Uint{1} << n);
}

Uint bit_toggle(Uint number, Uint n) {
    return number ^ Uint{1} << n;
}

bool bit_check(Uint number, Uint n) {
    return (number >> n) & Uint{1};
}

Uint bit_set(Uint number, Uint n, bool x) {
    return (number & ~(Uint{1} << n)) | (Uint{x} << n);
}

Uint bit_set_bad_1(Uint number, Uint n, bool x) {
    return number ^ (-Uint{x} ^ number) & (Uint{1} << n);
}

Uint bit_set_bad_2(Uint number, Uint n, bool x) {
    return number ^ (-Uint{x} ^ number) & (Uint{x} << n);
}

#include <iostream>
#include <string>

int main()
{
  Uint i = 0;

  i = bit_set(i, 2);
  i = bit_clear(i, 2);
  i = bit_toggle(i, 2);
  std::cout << "bit=" << bit_check(i,2) << std::endl;
  i = bit_set(i, 2, false);
  std::cout << "bit=" << bit_check(i,2) << std::endl;
}

