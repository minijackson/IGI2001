CC=ccache gcc
TEX=pdflatex
CFLAGS=-Wall -Wextra -pedantic -pthread -std=c99 -O2

all: exo1 exo2 exo3 exo4

exo1: exo1.c
	$(CC) $@.c -o $@ $(CFLAGS)

exo2: exo2.c
	$(CC) $@.c -o $@ $(CFLAGS)

exo3: exo3.c
	$(CC) $@.c -o $@ $(CFLAGS)

exo4: exo4.c
	$(CC) $@.c -o $@ $(CFLAGS)

report: rapport.tex
	$(TEX) rapport.tex

clean:
	rm -I exo? rapport.pdf rapport.aux rapport.log

.PHONY: all clean
