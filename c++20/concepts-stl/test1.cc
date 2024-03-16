#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

// https://en.cppreference.com/w/cpp/iterator/sortable

template<typename T> requires std::sortable<T>
void Sort(T &t) 
{
  std::ranges::sort(t);
}

int main()
{
  std::vector v { 3, 2, 1, 4, 5};
  Sort(v);
}
