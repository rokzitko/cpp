#include <iostream>
#include <string>
#include <random>
#include <complex>
#include <cmath>

#include <Eigen/Dense>

// https://eigen.tuxfamily.org/dox/group__QuickRefPage.html

int main()
{
  // Eigen::Matrix2cd
  Eigen::Matrix<std::complex<double>, 2, 2> m;

  m << 2, std::complex<double>(1,1), std::complex<double>(1,-1), 2;

  std::cout << "m=\n" << m << std::endl;

// https://eigen.tuxfamily.org/dox/group__Eigenvalues__Module.html
// https://eigen.tuxfamily.org/dox/classEigen_1_1SelfAdjointEigenSolver.html
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix2cd> es(m, Eigen::EigenvaluesOnly);
  std::cout << "lambda=\n" <<  es.eigenvalues() << std::endl;
}

