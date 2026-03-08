CXXFLAGS=-std=c++17

all: test1

test1: test1.cc  $(wildcard *.hh)
	${CXX} ${CXXFLAGS} $< -o $@

clean:
	rm -rf test1
