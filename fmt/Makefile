CXXFLAGS=-I. -std=c++14

all: test1

format.o: format.cc
	g++ ${CXXFLAGS} -c format.cc

test1: test1.cc format.o
	g++ ${CXXFLAGS} test1.cc format.o -o test1

clean:
	rm -rf test1 format.o
