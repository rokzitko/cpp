cmake_minimum_required(VERSION 3.10)

project(gsl_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic)

find_package(GSL REQUIRED)
link_libraries(GSL::gsl GSL::gslcblas)

add_executable(gsl_test gsl_test.cc)
