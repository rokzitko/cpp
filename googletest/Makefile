CXXFLAGS=-std=c++14 -I${HOME}/repos/googletest/googletest/include -L${HOME}/repos/googletest/build/lib -lgtest -lgtest_main -lpthread

all: test1

test1: test1.cc
	g++  test1.cc -o test1 ${CXXFLAGS}

clean:
	rm -rf test1
