#include <iostream>
#include <complex>
#include <vector>
#include <array>
#include <boost/math/quadrature/gauss.hpp>

using namespace std;
using namespace boost::math::quadrature;

constexpr int MAX_DEPTH = 30;
array<double, MAX_DEPTH> max_error = {};

// Define Function to Integrate
complex<double> f(double kx, double ky) {
    complex<double> omega(2.0, 1e-4);
    return (4.0 * M_PI * M_PI) / (omega + 2.0 * (cos(kx * M_PI) + cos(ky * M_PI)));
}

complex<double> integrate2(complex<double> (*func)(double, double), 
                           double x_min, double y_min, 
                           double x_max, double y_max) {
  constexpr auto order = 2;
  constexpr double points[] = { -0.5773502691896258, 0.5773502691896258 };
  constexpr double weights[] = { -1, 1 };
  
    complex<double> integral = 0.0;
    double J = 0.25 * (x_max - x_min) * (y_max - y_min);

    for (int i = 0; i < order; ++i) {
        double x_mapped = 0.5 * ((x_max - x_min) * points[i] + (x_max + x_min));
        for (int j = 0; j < order; ++j) {
            double y_mapped = 0.5 * ((y_max - y_min) * points[j] + (y_max + y_min));
            integral += weights[i] * weights[j] * func(x_mapped, y_mapped);
        }
    }
    return integral * J;
}

complex<double> integrate3(complex<double> (*func)(double, double), 
                           double x_min, double y_min, 
                           double x_max, double y_max) {
  constexpr auto order = 3;
  constexpr double points[] = { -0.7745966692414834,  0., 0.7745966692414834 };
  constexpr double weights[] = { 0.5555555555555558, 0.8888888888888883, 0.5555555555555558 };
  
    complex<double> integral = 0.0;
    double J = 0.25 * (x_max - x_min) * (y_max - y_min);

    for (int i = 0; i < order; ++i) {
        double x_mapped = 0.5 * ((x_max - x_min) * points[i] + (x_max + x_min));
        for (int j = 0; j < order; ++j) {
            double y_mapped = 0.5 * ((y_max - y_min) * points[j] + (y_max + y_min));
            integral += weights[i] * weights[j] * func(x_mapped, y_mapped);
        }
    }
    return integral * J;
}

// Adaptive Gauss-Cauchy Quadrature
complex<double> adaptive_gauss_cauchy(int depth, complex<double> (*func)(double, double),
                                      double x0, double y0, double x1, double y1,
                                      double tol = 1e-5) {
    complex<double> result1 = integrate2(func, x0, y0, x1, y1);
    complex<double> result2 = integrate3(func, x0, y0, x1, y1);
    double error_estimate = abs(result1 - result2);

    max_error[depth] = max(max_error[depth], error_estimate);

    if (error_estimate > tol) {
//        cout << "depth=" << depth << " x0=" << x0 << " x1=" << x1 
//             << " y0=" << y0 << " y1=" << y1 << " error=" << error_estimate << endl;
    }

    if (error_estimate < tol) {
        return result2;
    } else {
        double xm = (x0 + x1) / 2, ym = (y0 + y1) / 2;
        return adaptive_gauss_cauchy(depth + 1, func, x0, y0, xm, ym, tol) +
               adaptive_gauss_cauchy(depth + 1, func, xm, y0, x1, ym, tol) +
               adaptive_gauss_cauchy(depth + 1, func, x0, ym, xm, y1, tol) +
               adaptive_gauss_cauchy(depth + 1, func, xm, ym, x1, y1, tol);
    }
}

// Test Integration Function
void test_integration() {
    std::cout << "f(0,0) = " << f(0,0) << std::endl;

    double x0 = 0, y0 = 0, x1 = 1, y1 = 1;
    complex<double> result = adaptive_gauss_cauchy(0, f, x0, y0, x1, y1);

    cout << "Adaptive Gauss-Cauchy Quadrature Result: " << result << endl;

    complex<double> ref(10.591600401055332972, -13.549702328041742033);
    complex<double> diff = result - ref;

    cout << "diff=" << diff << endl;

    for (size_t o = 0; o < MAX_DEPTH; ++o) {
        if (max_error[o] > 0) {
            cout << o << " " << max_error[o] << endl;
        }
    }
}

int main() {
    test_integration();
    return 0;
}

