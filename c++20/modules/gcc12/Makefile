CXXFLAGS=-std=c++20 -fmodules-ts -x c++-system-header iostream -Wall -Wextra -pedantic

all: test1

test1: test1.cc
	${CXX} ${CXXFLAGS} -x c++ hello.cppm test1.cc -o test1

clean:
	rm test1
