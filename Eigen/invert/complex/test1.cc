#include <iostream>
#include <string>
#include <random>
#include <complex>
#include <cmath>

#include <Eigen/Dense>

// https://eigen.tuxfamily.org/dox/group__QuickRefPage.html

int main()
{
  Eigen::Matrix<std::complex<double>, 2, 2> m;

  m << 2, std::complex<double>(1,1), std::complex<double>(1,-1), 2;

  std::cout << "m=\n" << m << std::endl;

  auto inv = m.inverse();

  std::cout << "inv(m)=\n" << inv << std::endl;
}

