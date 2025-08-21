#include <iostream>
#include <deque>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

namespace nb = nanobind;
using namespace nb::literals;

struct el {
  int value;
  std::string str;

  int get_value() { return value; }
  std::string get_string() { return str; }
};

int add(int a, int b) { return a + b; }

using IntVector = std::vector<int>;

IntVector double_it(const IntVector &in) {
    IntVector out(in.size());
    for (size_t i = 0; i < in.size(); ++i)
        out[i] = in[i] * 2;
    return out;
}

void dump_deque(std::deque<el> l)
{
  for (auto x: l)
    std::cout << x.get_string() << std::endl;
}

void dump(std::vector<el> l)
{
  std::deque<el> d(l.begin(), l.end());
  dump_deque(d);
}

NB_MODULE(my_ext, m) {
  m.doc() = "A simple example python extension";

  m.def("double_it", &double_it);

  m.def("dump", &dump);

  m.def("add", &add, "a"_a, "b"_a = 1,
          "This function adds two numbers and increments if only one is provided.");

  m.attr("the_answer") = 42;

  nb::class_<el>(m, "el")
        .def(nb::init<>())
        .def(nb::init<int, const std::string &>())
        .def("get_value", &el::get_value)
        .def("get_string", &el::get_string)
        .def_rw("value", &el::value)
        .def_rw("str", &el::str)
        .def("__repr__",
         [](const el &p) { return "<my_ext.el str='" + p.str + "'>"; });
}
