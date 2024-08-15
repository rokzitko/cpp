CXXFLAGS=-std=c++17 -lcrypto

all: test1 test2

test1: test1.cc
	${CXX} ${CXXFLAGS} $< -o $@

test2: test2.cc
	${CXX} ${CXXFLAGS} $< -o $@

compare: test1 test2
	./test1
	./test2
	sha256sum file1
	sha256sum file2
	sha256sum file3
	sha256sum file4

clean:
	rm -rf test1 test2
