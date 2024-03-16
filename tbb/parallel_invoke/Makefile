CXXFLAGS=-Wall -Wextra -pedantic -std=c++17 -ltbb

all: test1

test1: test1.cc
	icpc ${CXXFLAGS} test1.cc -o test1

clean:
	rm -rf test1 log.out log.err
