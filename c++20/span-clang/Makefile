# note the setting -stdlib=libc++
CXXFLAGS=-std=c++2a -Wall -Wextra -pedantic -stdlib=libc++

all: test1

test1: test1.cc
	clang++ ${CXXFLAGS} test1.cc -o test1

clean:
	rm test1
