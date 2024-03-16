CXXFLAGS=-Wall -Wextra -pedantic -std=c++17 -ltbb -I.

all: test1

format.o: format.cc
	icpc ${CXXFLAGS} -c format.cc

test1: test1.cc format.o
	icpc ${CXXFLAGS} test1.cc format.o -o test1

clean:
	rm -rf test1 format.o
