#include <iostream>
#include <string>

template<typename T, size_t N>
constexpr auto array_size (T (&)[N]) -> size_t {
  return N;
}

int main()
{
  std::cout << "Hello world!" << std::endl;

  int arr[] = { 1, 3, 3, 7 };
  constexpr auto sz = array_size (arr);

  std::cout << "sz=" << sz << std::endl;
}

