CC=ccache gcc
CFLAGS=-Wall -Wextra -pedantic -pthread -std=c99 -O2

all:
	make exo1
	make exo2
	make exo3

exo1:
	$(CC) $@.c -o $@ $(CFLAGS)

exo2:
	$(CC) $@.c -o $@ $(CFLAGS)

exo3:
	$(CC) $@.c -o $@ $(CFLAGS)

clean:
	rm -I exo?
