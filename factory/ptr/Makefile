CXXFLAGS=-std=c++17

all: test1 test2 test3

test1: test1.cc
	g++ ${CXXFLAGS} $< -o $@

test2: test2.cc
	g++ ${CXXFLAGS} $< -o $@

clean:
	rm -rf test1 test2
