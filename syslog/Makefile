CXXFLAGS=-I. -std=c++14 -pthread

all: test1

log.o: log.cc
	g++ ${CXXFLAGS} -c log.cc

test1: test1.cc log.o
	g++ ${CXXFLAGS} test1.cc log.o -o test1

clean:
	rm -rf test1 log.o
