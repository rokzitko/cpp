CXXFLAGS=-std=c++23 -Wl,-rpath ${LD_LIBRARY_PATH} -Wall -fmodules-ts

.PHONY: all

SRCS := $(wildcard *.cc)
PRGS := $(patsubst %.cc,%,$(SRCS))

all: $(PRGS)

$(PRGS): %: %.cc
	${CXX} ${CXXFLAGS} $< -o $@

clean:
	rm -rf $(PRGS)
