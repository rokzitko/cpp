CXX      = g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra
LUA_PREFIX := $(shell brew --prefix lua)
LDLIBS   := $(LUA_PREFIX)/lib/liblua.a
CPPFLAGS := $(shell pkg-config --cflags lua)

all: test1

test1: test1.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) $< -o $@

clean:
	rm -rf test1
