CXXFLAGS=-std=c++17 -lboost_system 

all: test1

test1: test1.cc
	${CXX} ${CXXFLAGS} $< -o $@ -lpthread

clean:
	rm -rf test1
