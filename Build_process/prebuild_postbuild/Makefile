CXXFLAGS=-std=c++17

.PHONY: all clean

all: post-build

pre-build:
	@echo PRE
	gcc --version

post-build: main-build
	@echo POST

main-build: pre-build
	@echo MAIN
	@$(MAKE) --no-print-directory target

TESTS=test1

target: $(TESTS)

test1: test1.cc
	${CXX} ${CXXFLAGS} $< -o $@

clean:
	rm -rf $(TESTS)
