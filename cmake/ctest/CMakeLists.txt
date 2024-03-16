cmake_minimum_required (VERSION 3.10)
project (HELLO)

add_executable(test1 test1.cc)

enable_testing()
add_test(NAME prvi_test COMMAND test1 args)
add_test(NAME drugi_test COMMAND test1 diff_args)
