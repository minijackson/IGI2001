DIRS=$(wildcard TP?)

all:
	for dir in $(DIRS); do \
		$(MAKE) -C $$dir all; \
	done

report:
	for dir in $(DIRS); do \
		$(MAKE) -C $$dir report; \
	done

clean:
	for dir in $(DIRS); do \
		$(MAKE) -C $$dir clean; \
	done

tarball:
	for dir in $(DIRS); do \
		$(MAKE) -C $$dir tarball; \
	done

.PHONY: all clean tarball
