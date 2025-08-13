CXXFLAGS=-std=c++20 -I${EBROOTEIGEN}/include -lboost_serialization -lboost_mpi -lmpi

all: test1

test1: test1.cc
	g++ ${CXXFLAGS} test1.cc -o test1

clean:
	rm test1
