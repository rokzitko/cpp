#include <iostream>
#include <string>
#include <range/v3/all.hpp>

int main()
{
  {
    std::string s{"hello"};
    ranges::for_each(s, [](char c) { std::cout << c << ' '; });
    std::cout << std::endl;
  }

  {
    std::vector<int> v{6, 2, 3, 4, 5, 6};
    std::cout << std::boolalpha;
    std::cout << "vector: " << ranges::views::all(v) << '\n';
  }

  {
    auto const v = std::vector<int> {1,2,3,4,5,6};
    auto rng = v | ranges::views::all;
    std::cout << rng << std::endl;
  }

  // https://www.walletfox.com/course/quickref_range_v3.php
  {
    auto letters = ranges::views::closed_iota('A','C');
    auto numbers = ranges::views::closed_iota(1,3);

    auto rng = ranges::views::cartesian_product(letters, numbers);
    for(auto[a,b] : rng)
      std::cout << a << ' ' << b << '\n';
  }

  {
    auto const v = std::vector<double> {1.5,2.7,3.8,4.2};
    auto val = ranges::accumulate(v, 0.0);
    std::cout << val << std::endl;
  }

  {
    auto const v = std::vector<int> {1,2,3,4};
    auto val = ranges::accumulate(v, 1, [](int a, int b){return a * b;}); // 24
    std::cout << val << std::endl;
  }

  {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

    auto results = numbers | ranges::views::filter([](int n){ return n % 2 == 0; })
      | ranges::views::transform([](int n){ return n * 2; });

    for (auto v: results) std::cout << v << " ";
    std::cout << std::endl;
  }

  {
    auto x = ranges::views::closed_iota(1,10);
    auto vec = x | ranges::to<std::vector>;
    for (auto v: vec) std::cout << v << " ";
    std::cout << std::endl;

    auto y = x | ranges::views::take(5);
    std::cout << y << std::endl;
  }

// https://en.cppreference.com/w/cpp/ranges/drop_view
  {
    std::vector nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (int i : nums | ranges::views::drop(2))
      std::cout << i << ' ';
    std::cout << '\n';

    for (int i : ranges::views::iota(1, 10) | ranges::views::drop(2))
      std::cout << i << ' ';
    std::cout << '\n';
  }

  // https://en.cppreference.com/w/cpp/ranges
  {  std::vector<int> ints{0,1,2,3,4,5};
    auto even = [](int i){ return 0 == i % 2; };
    auto square = [](int i) { return i * i; };

    for (int i : ints | ranges::views::filter(even) | ranges::views::transform(square)) 
        std::cout << i << ' ';
    std::cout << std::endl;
  }

  {
    auto v123 = ranges::views::closed_iota(1,3);
    auto v456 = ranges::views::closed_iota(4,6);

    auto cyc123 = v123 | ranges::views::cycle;
    auto cyc456 = v456 | ranges::views::cycle;

    auto x = cyc123 | ranges::views::take(20);
    auto y = cyc456 | ranges::views::take(20);

    std::cout << x << std::endl;
    std::cout << y << std::endl;

    auto cc = ranges::views::concat(v123, v456);

    std::cout << cc << std::endl;
  }
  
  {
    auto x = ranges::views::closed_iota(1,20);
    auto c = x | ranges::views::chunk(3);
    std::cout << c << std::endl;
    for (auto a : c) 
      std::cout << a << std::endl;
  }
}
