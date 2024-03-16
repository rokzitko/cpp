CXXFLAGS=-std=c++11 -I/usr/include -pthread

all: server

test1: server.cc
	g++ ${CXXFLAGS} server.cc -o server

clean:
	rm server