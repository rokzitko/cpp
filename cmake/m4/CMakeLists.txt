cmake_minimum_required (VERSION 3.10)
project (HELLO)

set(test1_SRCS test1.cc)
add_executable(test1 ${test1_SRCS})

add_custom_target(test2.cc ALL m4 "-DTHIS=Hello" test2.cc.m4 ">test2.cc")

message(STATUS "Hello world!")
