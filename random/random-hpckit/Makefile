CXXFLAGS=-I. -std=c++14

all: test1

test1: test1.cc
	LANG=C singularity run ${HOME}/containers.singularity/oneapi-hpckit/oneapi-hpckit_beta07.sif icpc ${CXXFLAGS} test1.cc -o test1

clean:
	rm -rf test1
