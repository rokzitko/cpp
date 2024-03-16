CXXFLAGS=-Wall -Wextra -pedantic -std=c++17 -ltbb -DTBB_USE_DEBUG=1

all: test1

test1: test1.cc
	icpc ${CXXFLAGS} test1.cc -o test1 -ltbb

clean:
	rm -rf test1 log.out log.err
