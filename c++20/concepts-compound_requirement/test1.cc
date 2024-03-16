#include <iostream>
#include <string>

template<typename T>
  concept C = requires(T &x, T &y) {
    { x.swap(y) } noexcept;
    { x.size() } -> std::convertible_to<std::size_t>;
  };

int main()
{
}
