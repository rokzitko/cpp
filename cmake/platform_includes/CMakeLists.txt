cmake_minimum_required (VERSION 3.10)
project (HELLO)

set(test1_SRCS test1.cc)
add_executable(test1 ${test1_SRCS})

configure_file(test.h.cmake test.h)
