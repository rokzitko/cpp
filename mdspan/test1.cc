#include <experimental/mdspan>

#include <iostream>
#include <memory>
#include <cassert>

// row-major: fast-running is right index (C, C++, Python), layout_right (default!!)
// column-major: fast-running index is left index (Fortran, Julia, R, Matlab), layout_left

namespace stdex = std::experimental;

template<typename T, typename Exts, typename Lay, typename Acc>
  void dump2(stdex::basic_mdspan<T, Exts, Lay, Acc> a) requires Exts::rank() == 2
{
   for (int i = 0; i < a.extent(0); i++)
     for (int j = 0; j < a.extent(1); j++)
       std::cout << a(i, j) << (j < a.extent(1)-1 ? ' ' : '\n');
}

template<typename T, unsigned long I>
  void dump1(stdex::mdspan<T, I> a)
{
  for (int i = 0; i < a.extent(0); i++)
    std::cout << a(i) << (i < a.extent(0)-1 ? ' ' : '\n');
}

int main() {
   {
     int data[2*3*4] = {};
     auto a = stdex::mdspan<int, 2, 3, 4>(data);
     a(0,0,0) = 1;
     a(1,1,1) = 2;
     auto sub = stdex::submdspan(a, 1, 1, stdex::full_extent);
     std::cout << std::boolalpha << (sub[1] == 2) << std::endl;
   }
   {
     double data[2*3*4] = {};
     auto a = stdex::basic_mdspan<double, stdex::extents<2,3,4>, stdex::layout_left>(data);
     a(1,1,1) = 2;
     auto sub1 = stdex::submdspan(a, 1, stdex::full_extent, stdex::full_extent);
     auto sub2 = stdex::submdspan(sub1, 1, stdex::full_extent);
     std::cout << std::boolalpha << (sub2[1] == 2) << std::endl;
   }
   {
     double data[2*3*4] = {};
     auto a = stdex::basic_mdspan<double, stdex::extents<2,3,4>, stdex::layout_left>(data);
     a(1,1,1) = 2;
     auto sub = stdex::submdspan(a, 1, 1, 1);
     std::cout << std::boolalpha << (sub() == 2) << std::endl;
   }
   {
     double data[2*3] = { 1, 2, 3, 4, 5, 6 };
     auto a = stdex::basic_mdspan<double, stdex::extents<2,3>, stdex::layout_left>(data);
     dump2(a);
   }
   {
     double data[2*3] = { 1, 2, 3, 4, 5, 6 };
     auto a = stdex::basic_mdspan<double, stdex::extents<2,3>, stdex::layout_right>(data);
     dump2(a);
   }
   {
     double data[2*3] = {};
     auto a = stdex::mdspan<double, 2, 3>(data);
     assert(a.extent(0) == 2);
     assert(a.extent(1) == 3);
   }
  {
    double data[3] = { 1, 2, 3 };
    auto a = stdex::mdspan<double, 3>(data);
    dump1(a);
  }
  {
    double data[2*3] = {};
    auto a = stdex::mdspan<double, stdex::dynamic_extent, stdex::dynamic_extent>(data, 2, 3);
    a(1,1) = 42;
    std::cout << std::boolalpha << (a(1,1) == 42) << std::endl;
  }
}
