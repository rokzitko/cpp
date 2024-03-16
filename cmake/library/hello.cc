#include <iostream>
#include <string>
#include <random>

void test()
{
  int seed = 1234;

  std::default_random_engine dre(seed);

  std::uniform_int_distribution<int> di(10,20);
  for (int i = 0; i < 20; i++)
    std::cout << di(dre) << " ";
  std::cout << std::endl;

  std::uniform_real_distribution<double> dr(10.0,20.0);
  for (int i = 0; i < 20; i++)
    std::cout << dr(dre) << " ";
  std::cout << std::endl;
}

