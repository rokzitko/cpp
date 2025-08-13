CXXFLAGS=-std=c++2c

all: test1

test1: test1.cc
	./prepare
	${CXX} ${CXXFLAGS} $< -o $@

clean:
	rm -rf test1 random.bin

