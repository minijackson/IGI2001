CC       = ccache gcc
TEX      = pdflatex
CFLAGS   = -Wall -Wextra -pedantic -pthread -std=c99 -O2
SOURCES  = $(wildcard *.c)
EXECS    = $(SOURCES:%.c=%)
X11LIB   = `pkg-config --libs x11`
X11FLAGS = `pkg-config --cflags x11`
MATH     = -lm

all: $(EXECS)

$(%.c:%.c=%): %.c
	$(CC) $(CFLAGS) $@.c -o $@

report: rapport.tex
	$(TEX) rapport.tex

clean:
	rm exo? rapport.pdf rapport.aux rapport.log

tarball:
	tar zcvf $(notdir $(shell pwd)).tar.gz exo* out* *.pdf *.tex Makefile

.PHONY: all clean tarball
