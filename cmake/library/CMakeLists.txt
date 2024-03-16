cmake_minimum_required (VERSION 3.10)
project (HELLO)

add_library(Hello hello.cc)
add_executable(test1 test1.cc)
target_link_libraries(test1 LINK_PUBLIC Hello)
