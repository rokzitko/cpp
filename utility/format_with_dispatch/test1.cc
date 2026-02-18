#include <iostream>
#include <iomanip>
#include <string>

#include "format_with_dispatch.hh"

int main()
{
  std::cout << "Hello world!" << std::endl;
  FormatDispatch d;

  d['a'] = [](std::string_view p) {
    return p.empty() ? "A" : ("A(" + std::string(p) + ")");
  };

  d['b'] = [](std::string_view p) {
    int w = 0;
    if (!p.empty()) w = std::stoi(std::string(p));
    std::string s = "bee";
    if (w > 0 && static_cast<int>(s.size()) < w) s.insert(s.begin(), w - s.size(), ' ');
    return s;
  };

  d['c'] = [](std::string_view p) { return "C:" + std::string(p); };

  std::cout << format_with_dispatch("x=%a y=%12b z=%fooc\n", d);
}
