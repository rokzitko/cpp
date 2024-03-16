CXXFLAGS=-O3 -Wall -Wextra -pedantic -std=c++17 -I./include -DDHIGHFIVE_USE_BOOST=OFF -lhdf5

all: test1 test2

test1: test1.cc
	g++ ${CXXFLAGS} test1.cc -o test1

test2: test2.cc
	g++ ${CXXFLAGS} test2.cc -o test2

clean:
	rm -rf test1 test2
