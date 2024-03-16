DIRS := eigenvalues invert

.PHONY: all clean

all clean:
	for dir in $(DIRS); do \
	    $(MAKE) -C $$dir -f Makefile $@; \
	done
