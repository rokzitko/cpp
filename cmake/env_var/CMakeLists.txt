cmake_minimum_required (VERSION 3.10)
project (HELLO)

set(test1_SRCS test1.cc)
add_executable(test1 ${test1_SRCS})

message(STATUS "PATH=" $ENV{PATH})
message(STATUS "MYVAR=" ${MYVAR})

get_cmake_property(P VARIABLES)
foreach(VAR in ${P})
message(STATUS "${VAR}=${${VAR}}" )
endforeach()
