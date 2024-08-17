CXXFLAGS=-std=c++23

.PHONY: all

all: test1

test1: test1.cc
	# https://lmod.readthedocs.io/en/latest/040_FAQ.html
	source $$MODULESHOME/init/bash ; module purge ; module load foss/2023b ; ${CXX} --version ; ${CXX} ${CXXFLAGS} $< -o $@

clean:
	rm -rf test1
