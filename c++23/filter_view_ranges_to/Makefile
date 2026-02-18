CXXFLAGS=-std=c++23 -Wall
#-Wl,-rpath ${LD_LIBRARY_PATH} -Wall

.PHONY: all

SRCS := $(wildcard *.cc)
PRGS := $(patsubst %.cc,%,$(SRCS))

all: $(PRGS)

$(PRGS): %: %.cc
	g++-mp-15 ${CXXFLAGS} $< -o $@

clean:
	rm -rf $(PRGS)
